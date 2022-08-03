#include <QCompleter>
#include <QInputDialog>

#include "Utilities/Mission.h"
#include "Utilities/Validators/QStringListValidator.h"
#include "Windows/CarrierWindow.h"

CarrierWindow::CarrierWindow(QWidget* parent,
                             const std::map<const std::string, const std::vector<Group>>& mission_data,
                             const std::function<void(Carrier)>& on_ok,
                             const std::function<void()>& on_cancel)
	: QDialog(parent), m_mission_data(mission_data), m_on_ok(on_ok), m_on_cancel(on_cancel)
{
	m_ui.setupUi(this);

	m_ui.name->setCompleter(new QCompleter(Mission::DataToUnitName(m_mission_data)));
	m_ui.tanker->setCompleter(new QCompleter(Mission::DataToUnitName(m_mission_data)));
	m_ui.marshall_relay->setCompleter(new QCompleter(Mission::DataToUnitName(m_mission_data)));
	m_ui.lso_relay->setCompleter(new QCompleter(Mission::DataToUnitName(m_mission_data)));

	m_ui.base_frequency->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$")));
	m_ui.marshall_frequency->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$")));
	m_ui.lso_frequency->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$")));
	m_ui.tacan_morse->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Z]{3}$")));
	m_ui.icls_morse->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Z]{3}$")));

	connect(m_ui.add_recovery_btn, &QPushButton::clicked, this, &CarrierWindow::onRecoveryAddClicked);
	connect(m_ui.rm_recovery_btn, &QPushButton::clicked, this, &CarrierWindow::onRecoveryRemoveClicked);
	connect(m_ui.ok_btn, &QPushButton::clicked, this, &CarrierWindow::onOkClicked);
	connect(m_ui.cancel_btn, &QPushButton::clicked, this, &CarrierWindow::onCancelClicked);
}

CarrierWindow::~CarrierWindow()
{
	delete m_ui.name->completer();
	delete m_ui.tanker->completer();
	delete m_ui.marshall_relay->completer();
	delete m_ui.lso_relay->completer();

	delete m_ui.base_frequency->validator();
	delete m_ui.marshall_frequency->validator();
	delete m_ui.lso_frequency->validator();
	delete m_ui.tacan_morse->validator();
	delete m_ui.icls_morse->validator();
}

void CarrierWindow::SetCarrier(const Carrier& carrier) const
{
	m_ui.coalition->setCurrentIndex(m_ui.coalition->findText(Coalition::ToString(carrier.Coalition).c_str()));
	m_ui.name->setText(carrier.Name.c_str());
	m_ui.alias->setText(carrier.Alias.c_str());
	m_ui.tanker->setText(carrier.RecoveryTanker.c_str());

	m_ui.base_frequency->setText(carrier.BaseFq.c_str());
	m_ui.marshall_frequency->setText(carrier.MarshallFq.c_str());
	m_ui.lso_frequency->setText(carrier.LsoFq.c_str());
	m_ui.marshall_relay->setText(carrier.MarshallRelayUnit.c_str());
	m_ui.lso_relay->setText(carrier.LsoRelayUnit.c_str());

	m_ui.control_area->setValue(carrier.ControlArea);
	m_ui.max_patterns->setValue(carrier.MaxPatterns);
	m_ui.recovery_mode->setValue(carrier.RecoveryMode);
	m_ui.max_stacks->setValue(carrier.MaxStacks);

	m_ui.menu_mark_zone->setChecked(carrier.MenuMarkZone);
	m_ui.menu_smoke_zone->setChecked(carrier.MenuSmokeZone);
	m_ui.nice_guy->setChecked(carrier.NiceGuy);
	m_ui.handle_ai->setChecked(carrier.HandleAi);
	m_ui.infinite_patrol->setChecked(carrier.InfinitePatrol);
	m_ui.single_carrier->setChecked(carrier.SingleCarrier);

	m_ui.difficulty->setCurrentIndex(m_ui.difficulty->findText(carrier.AirbossDifficulty.c_str()));
	m_ui.tacan->setValue(carrier.Tacan.Channel);
	m_ui.tacan_band->setCurrentIndex(m_ui.tacan_band->findText(carrier.Tacan.Band.c_str()));
	m_ui.tacan_morse->setText(carrier.Tacan.Morse.c_str());
	m_ui.icls_channel->setValue(carrier.Icls.Channel);
	m_ui.icls_morse->setText(carrier.Icls.Morse.c_str());

	const QString recovery_type = carrier.RecoveryOps.Type == models::RecoveryOps::Type::Cyclic
		                              ? "Cyclic"
		                              : "Alpha Strike";
	m_ui.recovery_type->setCurrentIndex(m_ui.recovery_type->findText(recovery_type));
	m_ui.event_duration->setValue(carrier.RecoveryOps.EventDuration);
	m_ui.event_ia_duration->setValue(carrier.RecoveryOps.EventIaDuration);

	for (const auto& recovery : carrier.RecoveryOps.Recoveries)
		m_ui.recoveries->addItem(std::format("Start: {}, Duration: {}, Case: {}",
		                                     recovery.Start,
		                                     recovery.Duration,
		                                     recovery.RecoveryMode).c_str());

	m_ui.stat_path->setText(carrier.StatPath.c_str());
	m_ui.trapsheet_path->setText(carrier.TrapsheetsPath.c_str());
}

void CarrierWindow::onRecoveryAddClicked()
{
	bool is_ok = false;
	const int start = QInputDialog::getInt(this,
	                                       "Add Recovery",
	                                       "Recovery Start time:",
	                                       0,
	                                       0,
	                                       240,
	                                       1,
	                                       &is_ok);
	if (!is_ok)
	{
		LOG_INFO("User cancelled recovery add at first step with start = {}", start);
		return;
	}
	const int duration = QInputDialog::getInt(this,
	                                          "Add Recovery",
	                                          "Recovery duration:",
	                                          0,
	                                          0,
	                                          240,
	                                          1,
	                                          &is_ok);
	if (!is_ok)
	{
		LOG_INFO("User cancelled recovery add at second step with duration = {}", duration);
		return;
	}
	const int case_number = QInputDialog::getInt(this,
	                                             "Add Recovery",
	                                             "Recovery Case:",
	                                             0,
	                                             1,
	                                             3,
	                                             1,
	                                             &is_ok);
	if (!is_ok)
	{
		LOG_INFO("User cancelled recovery add at third step with case = {}", case_number);
		return;
	}

	m_ui.recoveries->addItem(std::format("Start: {}, Duration: {}, Case: {}",
	                                     start,
	                                     duration,
	                                     case_number).c_str());
	LOG_INFO("Recovery Start: {}, Duration: {}, Case: {}", start, duration, case_number);
}

void CarrierWindow::onRecoveryRemoveClicked() const
{
	qDeleteAll(m_ui.recoveries->selectedItems());
	LOG_INFO("Selected recoveries removed from carrier !");
}

void CarrierWindow::onOkClicked()
{
	std::vector<models::RecoveryOps::AlphaStrikeRecovery> recoveries;
	recoveries.reserve(m_ui.recoveries->count());
	for (int i = 0; i < m_ui.recoveries->count(); i++)
	{
		const std::string line = m_ui.recoveries->item(i)->text().toStdString();
		const std::string start = line.substr(line.find("Start: ") + 7);
		const std::string duration = line.substr(line.find("Duration: ") + 10);
		const std::string case_number = line.substr(line.find("Case: ") + 6);

		recoveries.emplace_back(std::stoi(start), std::stoi(duration), std::stoi(case_number));
	}

	try
	{
		Carrier carrier = {
			.Coalition = Coalition::FromString(m_ui.coalition->currentText().toStdString()),
			.Name = m_ui.name->text().toStdString(),
			.Alias = m_ui.alias->text().toStdString(),
			.RecoveryTanker = m_ui.tanker->text().toStdString(),
			.AirbossDifficulty = m_ui.difficulty->currentText().toStdString(),
			.BaseFq = m_ui.base_frequency->text().toStdString(),
			.MarshallFq = m_ui.marshall_frequency->text().toStdString(),
			.LsoFq = m_ui.lso_frequency->text().toStdString(),
			.MarshallRelayUnit = m_ui.marshall_relay->text().toStdString(),
			.LsoRelayUnit = m_ui.lso_relay->text().toStdString(),
			.StatPath = m_ui.stat_path->text().toStdString(),
			.TrapsheetsPath = m_ui.trapsheet_path->text().toStdString(),
			.MenuMarkZone = m_ui.menu_mark_zone->isChecked(),
			.MenuSmokeZone = m_ui.menu_smoke_zone->isChecked(),
			.NiceGuy = m_ui.nice_guy->isChecked(),
			.HandleAi = m_ui.handle_ai->isChecked(),
			.InfinitePatrol = m_ui.infinite_patrol->isChecked(),
			.SingleCarrier = m_ui.single_carrier->isChecked(),
			.ControlArea = m_ui.control_area->value(),
			.RecoveryMode = m_ui.recovery_mode->value(),
			.MaxPatterns = m_ui.max_patterns->value(),
			.MaxStacks = m_ui.max_stacks->value(),
			.RecoveryOps = {
				.Type = m_ui.recovery_type->currentText() == "Cyclic"
					        ? models::RecoveryOps::Type::Cyclic
					        : models::RecoveryOps::Type::AlphaStrike,
				.EventDuration = m_ui.event_duration->value(),
				.EventIaDuration = m_ui.event_ia_duration->value(),
				.Recoveries = recoveries
			},
			.MenuRecovery = {
				.Duration = 30,
				.WindOnDeck = 30,
				.Offset = 0,
				.UTurn = true
			},
			.Tacan = {
				.Channel = m_ui.tacan->value(),
				.Band = m_ui.tacan_band->currentText().toStdString(),
				.Morse = m_ui.tacan_morse->text().toStdString()
			},
			.Icls = {
				.Channel = m_ui.icls_channel->value(),
				.Morse = m_ui.icls_morse->text().toStdString()
			}
		};

		if (!Mission::DataToUnitName(m_mission_data).contains(carrier.Name.c_str()))
			throw std::exception("Invalid name !");
		if (!Mission::DataToUnitName(m_mission_data).contains(carrier.MarshallRelayUnit.c_str()))
			throw std::exception("Invalid marshall relay !");
		if (!Mission::DataToUnitName(m_mission_data).contains(carrier.LsoRelayUnit.c_str()))
			throw std::exception("Invalid lso relay !");
		if (carrier.Alias.empty())
			throw std::exception("Alias can't be empty !");
		//if (!carrier.RecoveryTanker.empty() || Mission::DataToGroupName(m_mission_data).
		//    contains(carrier.RecoveryTanker.c_str()))
		//	throw std::exception("Invalid recovery tanker !");
		if (carrier.BaseFq.size() != 7 && carrier.MarshallFq.size() != 7 && carrier.LsoFq.size() != 7)
			throw std::exception("Invalid radio frequency !");
		if (carrier.Tacan.Morse.size() != 3)
			throw std::exception("Invalid tacan morse code !");
		if (carrier.Icls.Morse.size() != 3)
			throw std::exception("Invalid icls morse code !");
		if (carrier.RecoveryOps.Type == models::RecoveryOps::Type::AlphaStrike
		    && carrier.RecoveryOps.Recoveries.empty())
			throw std::exception("No recovery selected !");

		close();
		std::invoke(m_on_ok, carrier);
	} catch (const std::exception& except)
	{
		m_ui.error_msg->setText(except.what());
		LOG_ERROR(except.what());
	}
}

void CarrierWindow::onCancelClicked()
{
	close();
	std::invoke(m_on_cancel);
}
