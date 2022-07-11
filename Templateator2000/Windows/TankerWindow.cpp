#include <QCompleter>

#include "Utilities/Coalition.h"
#include "Utilities/Mission.h"
#include "Utilities/Moose.h"
#include "Utilities/Validators/QStringListValidator.h"
#include "Windows/TankerWindow.h"

TankerWindow::TankerWindow(QWidget* parent,
                           const std::map<const std::string, const std::vector<Group>>& mission_data,
                           const std::function<void(Tanker)>& on_ok,
                           const std::function<void()>& on_cancel)
	: QDialog(parent), m_mission_data(mission_data), m_on_ok(on_ok), m_on_cancel(on_cancel)
{
	m_ui.setupUi(this);

	m_ui.callsign->addItems(Moose::GetQtTankerCallsigns());

	m_ui.departure->setCompleter(new QCompleter(Moose::GetQtAirbases(), this));
	m_ui.parking_size->setCompleter(new QCompleter(Moose::GetQtParkings(), this));
	m_ui.pattern->setCompleter(new QCompleter(Mission::DataToUnitName(m_mission_data), this));
	m_ui.group->setCompleter(new QCompleter(Mission::DataToGroupName(m_mission_data), this));
	m_ui.escort->setCompleter(new QCompleter(Mission::DataToGroupName(m_mission_data), this));

	m_ui.frequency->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$")));
	m_ui.tacan_morse->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Z]{3}$")));
	m_ui.departure->setValidator(new QStringListValidator(Moose::GetQtAirbases()));
	m_ui.parking_size->setValidator(new QStringListValidator(Moose::GetQtParkings()));
	m_ui.pattern->setValidator(new QStringListValidator(Mission::DataToUnitName(m_mission_data)));
	m_ui.group->setValidator(new QStringListValidator(Mission::DataToGroupName(m_mission_data)));
	m_ui.escort->setValidator(new QStringListValidator(Mission::DataToGroupName(m_mission_data)));

	connect(m_ui.ok_btn, &QPushButton::clicked, this, &TankerWindow::onOkClicked);
	connect(m_ui.cancel_btn, &QPushButton::clicked, this, &TankerWindow::onCancelClicked);
}

TankerWindow::~TankerWindow()
{
	delete m_ui.departure->completer();
	delete m_ui.parking_size->completer();
	delete m_ui.pattern->completer();
	delete m_ui.group->completer();
	delete m_ui.escort->completer();

	delete m_ui.frequency->validator();
	delete m_ui.departure->validator();
	delete m_ui.tacan_morse->validator();
	delete m_ui.parking_size->validator();
	delete m_ui.pattern->validator();
	delete m_ui.group->validator();
	delete m_ui.escort->validator();
}

void TankerWindow::SetTanker(const Tanker& tk) const
{
	m_ui.type->setCurrentIndex(m_ui.type->findText(tk.Type == Tanker::Type::Fixed ? "Fixed" : "On Demand"));
	m_ui.coalition->setCurrentIndex(m_ui.coalition->findText(Coalition::ToString(tk.Coalition).c_str()));
	m_ui.auto_respawn->setChecked(tk.AutoRespawn);
	m_ui.airboss_recovery->setChecked(tk.AirbossRecovery);
	m_ui.pattern->setText(tk.PatternUnit.c_str());
	m_ui.departure->setText(Moose::GetNameFromMooseAirbase(tk.DepartureBase).c_str());
	m_ui.parking_size->setText(tk.TerminalType.c_str());
	m_ui.group->setText(tk.GroupName.c_str());
	m_ui.escort->setText(tk.EscortGroupName.c_str());
	m_ui.frequency->setText(tk.Frequency.c_str());
	m_ui.max_duration->setValue(tk.MaxMissionDuration);
	m_ui.altitude->setValue(tk.Altitude);
	m_ui.speed->setValue(tk.Speed);
	m_ui.fuel_level->setValue(tk.FuelWarningLevel);
	m_ui.modex->setValue(tk.Modex);

	m_ui.tacan->setValue(tk.Tacan.Channel);
	m_ui.tacan_band->setCurrentIndex(m_ui.tacan_band->findText(tk.Tacan.Band.c_str()));
	m_ui.tacan_morse->setText(tk.Tacan.Morse.c_str());

	m_ui.racetrack_front->setValue(tk.Racetrack.Front);
	m_ui.racetrack_back->setValue(tk.Racetrack.Back);

	m_ui.callsign->setCurrentIndex(m_ui.callsign->findText(tk.Callsign.Name.c_str()));
	m_ui.callsign_nb->setValue(tk.Callsign.Number);
}

void TankerWindow::onOkClicked()
{
	try
	{
		Tanker tanker = {
			.Type = m_ui.type->currentText() == "Fixed" ? Tanker::Type::Fixed : Tanker::Type::OnDemand,
			.Coalition = Coalition::FromString(m_ui.coalition->currentText().toStdString()),
			.AutoRespawn = m_ui.auto_respawn->isChecked(),
			.AirbossRecovery = m_ui.airboss_recovery->isChecked(),
			.PatternUnit = m_ui.pattern->text().toStdString(),
			.DepartureBase = Moose::GetMooseAirbaseFromName(m_ui.departure->text().toStdString()),
			.TerminalType = m_ui.parking_size->text().toStdString(),
			.GroupName = m_ui.group->text().toStdString(),
			.EscortGroupName = m_ui.escort->text().toStdString(),
			.Frequency = m_ui.frequency->text().toStdString(),
			.MaxMissionDuration = m_ui.max_duration->value(),
			.Altitude = m_ui.altitude->value(),
			.Speed = m_ui.altitude->value(),
			.FuelWarningLevel = m_ui.fuel_level->value(),
			.Modex = m_ui.modex->value(),
			.Tacan = {
				.Channel = m_ui.tacan->value(),
				.Band = m_ui.tacan_band->currentText().toStdString(),
				.Morse = m_ui.tacan_morse->text().toStdString(),
			},
			.Racetrack = {
				.Front = m_ui.racetrack_front->value(),
				.Back = m_ui.racetrack_back->value(),
			},
			.Callsign = {
				.Name = m_ui.callsign->currentText().toStdString(),
				.Number = m_ui.callsign_nb->value()
			}
		};

		if (!Mission::DataToUnitName(m_mission_data).contains(tanker.PatternUnit.c_str()))
			throw std::exception("Invalid pattern unit !");
		if (!tanker.EscortGroupName.empty() || Mission::DataToGroupName(m_mission_data).
		    contains(tanker.EscortGroupName.c_str()))
			throw std::exception("Invalid escort group !");
		if (tanker.Frequency.empty() || tanker.Frequency.size() != 7)
			throw std::exception("Invalid radio frequency !");
		if (tanker.Tacan.Morse.empty() || tanker.Tacan.Morse.size() != 3)
			throw std::exception("Invalid tacan morse code !");

		close();
		std::invoke(m_on_ok, tanker);
	} catch (const std::exception& except)
	{
		m_ui.error_msg->setText(except.what());
	}
}

void TankerWindow::onCancelClicked()
{
	close();
	std::invoke(m_on_cancel);
}
