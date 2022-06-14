#include "Utilities/Moose.h"

#include "TankerWindow.h"

TankerWindow::TankerWindow(QWidget* parent, const std::map<const std::string, const std::vector<Group>>& mission_data, std::function<void(Tanker)> on_ok, std::function<void(void)> on_cancel)
	: QDialog(parent), m_missionData(mission_data), m_onOk(on_ok), m_onCancel(on_cancel)
{
	m_ui.setupUi(this);

	m_ui.departure->addItems(Moose::GetQTAirbases());
	m_ui.parking_size->addItems(Moose::GetQTParkings());
	m_ui.callsign->addItems(Moose::GetQTTankerCallsigns());

	m_ui.frequency->setValidator(new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$")));
	m_ui.tacan_morse->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Z]{3}$")));

	connect(m_ui.ok_btn, &QPushButton::clicked, this, &TankerWindow::on_ok_clicked);
	connect(m_ui.cancel_btn, &QPushButton::clicked, this, &TankerWindow::on_cancel_clicked);
}

TankerWindow::~TankerWindow()
{
	delete m_ui.frequency->validator();
	delete m_ui.tacan_morse->validator();
}

void TankerWindow::SetTanker(const Tanker& tk)
{
	m_ui.type->setCurrentIndex(m_ui.type->findText(tk.Type == Tanker::Type::Fixed ? "Fixed" : "On Demand"));
	m_ui.coalition->setCurrentIndex(m_ui.coalition->findText(tk.Coalition == Coalition::Blue ? "Blue" : tk.Coalition == Coalition::Red ? "Red" : "Neutral"));
	m_ui.pattern->setCurrentIndex(m_ui.pattern->findText(tk.PatternUnit.c_str()));
	m_ui.departure->setCurrentIndex(m_ui.departure->findText(Moose::GetNameFromMooseAirbase(tk.DepartureBase).c_str()));
	m_ui.parking_size->setCurrentIndex(m_ui.parking_size->findText(Moose::GetNameFromMooseParking(tk.ParkingSize).c_str()));
	m_ui.callsign->setCurrentIndex(m_ui.callsign->findText(Moose::GetNameFromMooseCallsign(tk.Callsign).c_str()));
	m_ui.frequency->setText(tk.Frequency.c_str());
	m_ui.tacan_morse->setText(tk.TacanMorse.c_str());
	m_ui.auto_respawn->setChecked(tk.AutoRespawn);
	m_ui.max_duration->setValue(tk.MaxMissionDuration);
	m_ui.altitude->setValue(tk.Altitude);
	m_ui.speed->setValue(tk.Speed);
	m_ui.fuel_level->setValue(tk.FuelWarningLevel);
	m_ui.modex->setValue(tk.Modex);
	m_ui.tacan->setValue(tk.TacanChannel);
	m_ui.racetrack_front->setValue(tk.RacetrackFront);
	m_ui.racetrack_back->setValue(tk.RacetrackBack);
	m_ui.callsign_nb->setValue(tk.CallsignNb);
}

void TankerWindow::on_ok_clicked()
{
	Tanker tanker = {
		.Type = m_ui.type->currentText() == "Fixed" ? Tanker::Type::Fixed : Tanker::Type::OnDemand,
		.Coalition = m_ui.coalition->currentText() == "Blue" ? Coalition::Blue : m_ui.coalition->currentText() == "Red" ? Coalition::Red : Coalition::Neutral,
		.PatternUnit = m_ui.pattern->currentText().toStdString(),
		.DepartureBase = Moose::GetMooseAirbaseFromName(m_ui.departure->currentText().toStdString()),
		.ParkingSize = Moose::GetMooseParkingFromName(m_ui.parking_size->currentText().toStdString()),
		.Callsign = Moose::GetMooseCallsignFromName(m_ui.callsign->currentText().toStdString()),
		.Frequency = m_ui.frequency->text().toStdString(),
		.TacanMorse = m_ui.tacan_morse->text().toStdString(),
		.AutoRespawn = m_ui.auto_respawn->isChecked(),
		.MaxMissionDuration = m_ui.max_duration->value(),
		.Altitude = m_ui.altitude->value(),
		.Speed = m_ui.altitude->value(),
		.FuelWarningLevel = m_ui.fuel_level->value(),
		.Modex = m_ui.modex->value(),
		.TacanChannel = m_ui.tacan->value(),
		.RacetrackFront = m_ui.racetrack_front->value(),
		.RacetrackBack = m_ui.racetrack_back->value(),
		.CallsignNb = m_ui.callsign_nb->value()
	};
	std::invoke(m_onOk, tanker);
}

void TankerWindow::on_cancel_clicked()
{
	close();
	std::invoke(m_onCancel);
}
