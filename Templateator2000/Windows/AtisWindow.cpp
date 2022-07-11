#include <QCompleter>
#include <QInputDialog>
#include <QMessageBox>

#include "Utilities/Mission.h"
#include "Utilities/Validators/QStringListValidator.h"
#include "Windows/AtisWindow.h"

AtisWindow::AtisWindow(QWidget* parent,
                       const std::map<const std::string, const std::vector<Group>>& mission_data,
                       const std::function<void(Atis)>& on_ok,
                       const std::function<void()>& on_cancel)
	: QDialog(parent), m_mission_data(mission_data), m_on_ok(on_ok), m_on_cancel(on_cancel)
{
	m_ui.setupUi(this);

	m_ui.airport->setCompleter(new QCompleter(Moose::GetQtAirbases(), this));
	m_ui.relay_unit->setCompleter(new QCompleter(Mission::DataToUnitName(m_mission_data), this));
	m_ui.frequency->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$")));

	m_ui.airport->setValidator(new QStringListValidator(Moose::GetQtAirbases()));
	m_ui.relay_unit->setValidator(new QStringListValidator(Mission::DataToUnitName(m_mission_data)));
	m_ui.power->setValidator(new QIntValidator(0, 100));

	connect(m_ui.ok_btn, &QPushButton::clicked, this, &AtisWindow::onOkClicked);
	connect(m_ui.cancel_btn, &QPushButton::clicked, this, &AtisWindow::onCancelClicked);
	connect(m_ui.add_frequency_btn, &QPushButton::clicked, this, &AtisWindow::onTowerFqAddClicked);
	connect(m_ui.rm_frequency_btn, &QPushButton::clicked, this, &AtisWindow::onTowerFqRemoveClicked);
}

AtisWindow::~AtisWindow()
{
	delete m_ui.airport->completer();
	delete m_ui.relay_unit->completer();
	delete m_ui.frequency->completer();

	delete m_ui.airport->validator();
	delete m_ui.relay_unit->validator();
	delete m_ui.power->validator();
}

void AtisWindow::SetAtis(const Atis& atis) const
{
	m_ui.airport->setText(atis.AirportName.c_str());
	m_ui.runway->setText(std::format("{}{}", atis.ActiveRunwayNumber, atis.ActiveRunwaySide).c_str());

	m_ui.frequency->setText(atis.Radio.Frequency.c_str());
	m_ui.modulation->setCurrentIndex(m_ui.modulation->findText(atis.Radio.Modulation.c_str()));
	m_ui.relay_unit->setText(atis.Radio.RelayUnit.c_str());
	for (const auto& fq : atis.Radio.TowerFrequencies)
		m_ui.tower_frequencies->addItem(fq.c_str());
	m_ui.power->setText(QString::number(atis.Radio.Power));

	m_ui.tacan->setValue(atis.Tacan.Channel);
	m_ui.tacan_band->setCurrentIndex(m_ui.tacan_band->findText(atis.Tacan.Band.c_str()));

	m_ui.ils_fq->setText(atis.Ils.Frequency.c_str());
	m_ui.ils_rwy->setText(atis.Ils.Runway.c_str());

	m_ui.srs_path->setText(atis.SrsPath.c_str());
}

void AtisWindow::onOkClicked()
{
	try
	{
		std::vector<std::string> tower_frequencies;
		tower_frequencies.reserve(m_ui.tower_frequencies->count());
		for (int i = 0; i < m_ui.tower_frequencies->count(); ++i)
			tower_frequencies.emplace_back(m_ui.tower_frequencies->item(i)->text().toStdString());

		const QString runway_label = m_ui.runway->text();
		const std::string runway_nb = !runway_label.back().isDigit()
			                              ? runway_label.toStdString()
			                              : runway_label.toStdString().substr(0, runway_label.size());
		const std::string runway_side = !runway_label.back().isDigit()
			                                ? std::string(runway_label.toStdString().back(), 1)
			                                : "";

		Atis atis = {
			.AirportName = m_ui.airport->text().toStdString(),
			.ActiveRunwayNumber = runway_nb,
			.ActiveRunwaySide = runway_side,
			.Radio = {
				.Frequency = m_ui.frequency->text().toStdString(),
				.Modulation = m_ui.modulation->currentText().toStdString(),
				.RelayUnit = m_ui.relay_unit->text().toStdString(),
				.TowerFrequencies = tower_frequencies,
				.Power = m_ui.power->text().toInt()
			},
			.Tacan = {
				.Channel = m_ui.tacan->text().toInt(),
				.Band = m_ui.tacan_band->currentText().toStdString(),
			},
			.Ils = {
				.Frequency = m_ui.ils_fq->text().toStdString(),
				.Runway = m_ui.ils_rwy->text().toStdString(),
			},
			.SrsPath = m_ui.srs_path->text().toStdString()
		};

		if (atis.AirportName.empty())
			throw std::exception("No Airport set !");
		if (atis.ActiveRunwayNumber.empty())
			throw std::exception("No active runway set !");
		if (atis.Radio.Frequency.empty())
			throw std::exception("No radio frequency set !");
		if (atis.Radio.RelayUnit.empty())
			throw std::exception("No relay unit set !");
		if (atis.Radio.Power == 0)
			throw std::exception("No radio power set (0 is not a valid value) !");
		if (atis.SrsPath.empty())
			throw std::exception("No SRS path set !");

		close();
		std::invoke(m_on_ok, atis);
	} catch (const std::exception& except)
	{
		m_ui.error_msg->setText(except.what());
		LOG_ERROR(except.what());
	}
}

void AtisWindow::onCancelClicked()
{
	close();
	std::invoke(m_on_cancel);
}

void AtisWindow::onTowerFqAddClicked()
{
	bool is_ok = false;
	QString res = QInputDialog::getText(this,
	                                    "GetAtis Dialogs",
	                                    "Enter Tower Frequency : ",
	                                    QLineEdit::Normal,
	                                    "",
	                                    &is_ok);

	if (is_ok && !res.isEmpty())
	{
		int pos = 0;
		if (m_ui.frequency->validator()->validate(res, pos) == QValidator::State::Invalid)
		{
			LOG_INFO("Error in GetAtis: {} is not a valid frequency !", res.toStdString());
			QMessageBox::warning(this, "Error", "Invalid frequency !");
			return;
		}
		m_ui.tower_frequencies->addItem(res);
		LOG_INFO("Tower frequency {} added to atis !", res.toStdString());
	}
}

void AtisWindow::onTowerFqRemoveClicked() const
{
	qDeleteAll(m_ui.tower_frequencies->selectedItems());
	LOG_INFO("Selected tower frequencies removed from atis !");
}
