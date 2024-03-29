#include <QCloseEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>

#include "Utilities/Log.h"
#include "Windows/AboutDialog.h"
#include "Windows/CarrierWindow.h"
#include "Windows/Wizards/Atis/AtisWizard.h"
#include "Windows/Wizards/Awacs/AwacsWizard.h"
#include "Windows/Wizards/Beacon/BeaconWizard.h"
#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	connect(m_ui.actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(m_ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(m_ui.actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(m_ui.actionExit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

	connect(m_ui.actionRefresh, SIGNAL(triggered()), this, SLOT(refresh()));
	connect(m_ui.actionManageScripts, SIGNAL(triggered()), this, SLOT(manageScripts()));

	connect(m_ui.actionUpdate, SIGNAL(triggered()), this, SLOT(updateScripts()));
	connect(m_ui.actionAddTanker, SIGNAL(triggered()), this, SLOT(addTanker()));
	connect(m_ui.actionAddCarrier, SIGNAL(triggered()), this, SLOT(addCarrier()));
	connect(m_ui.actionAddBeacon, SIGNAL(triggered()), this, SLOT(addBeacon()));
	connect(m_ui.actionAddAwacs, SIGNAL(triggered()), this, SLOT(addAwacs()));
	connect(m_ui.actionAddAtis, SIGNAL(triggered()), this, SLOT(addAtis()));

	connect(m_ui.actionHelp, SIGNAL(triggered()), this, SLOT(help()));
	connect(m_ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));

	connect(m_ui.tanker_add_btn, SIGNAL(clicked()), this, SLOT(addTanker()));
	connect(m_ui.carrier_add_btn, SIGNAL(clicked()), this, SLOT(addCarrier()));
	connect(m_ui.beacon_add_btn, SIGNAL(clicked()), this, SLOT(addBeacon()));
	connect(m_ui.awacs_add_btn, SIGNAL(clicked()), this, SLOT(addAwacs()));
	connect(m_ui.atis_add_btn, SIGNAL(clicked()), this, SLOT(addAtis()));

	connect(m_ui.tanker_rm_btn, SIGNAL(clicked()), this, SLOT(removeTanker()));
	connect(m_ui.carrier_rm_btn, SIGNAL(clicked()), this, SLOT(removeCarrier()));
	connect(m_ui.beacon_rm_btn, SIGNAL(clicked()), this, SLOT(removeBeacon()));
	connect(m_ui.awacs_rm_btn, SIGNAL(clicked()), this, SLOT(removeAwacs()));
	connect(m_ui.atis_rm_btn, SIGNAL(clicked()), this, SLOT(removeAtis()));

	connect(m_ui.tanker_edit_btn, SIGNAL(clicked()), this, SLOT(editTanker()));
	connect(m_ui.carrier_edit_btn, SIGNAL(clicked()), this, SLOT(editCarrier()));
	connect(m_ui.awacs_edit_btn, SIGNAL(clicked()), this, SLOT(editAwacs()));
	connect(m_ui.beacon_edit_btn, SIGNAL(clicked()), this, SLOT(editBeacon()));
	connect(m_ui.atis_edit_btn, SIGNAL(clicked()), this, SLOT(editAtis()));

	if (!std::filesystem::exists("temp"))
		std::filesystem::create_directory("temp");
}

MainWindow::~MainWindow()
{
	if (std::filesystem::exists("temp"))
		std::filesystem::remove_all("temp");
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (m_mission.IsInitialized() && !m_mission.IsSaved())
	{
		const auto res = QMessageBox::question(this,
		                                       "Warning",
		                                       "Mission is not saved, do you want to save it ?",
		                                       QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		if (res == QMessageBox::Yes)
		{
			save();
			event->accept();
		}
		else if (res == QMessageBox::No)
			event->accept();
		else
			event->ignore();
	}
	LOG_INFO("Exiting application.");
}

#pragma region TopBar
void MainWindow::open()
{
	const std::string filepath = QFileDialog::getOpenFileName(this, "Open File", "", "MIZ Files (*.miz)").toStdString();
	if (filepath.empty())
		return;

	loadMission(filepath);
}

void MainWindow::save()
{
	CHECK_MISSION_LOADED()
	try
	{
		m_mission.Save();
		LOG_TRACE("Mission {} saved sucessfully !", m_mission.GetMissionName());
	} catch (const std::exception& except)
	{
		QMessageBox::critical(nullptr, "Error", except.what(), QMessageBox::Ok);
		LOG_ERROR(except.what());
	}
}

void MainWindow::saveAs()
{
	CHECK_MISSION_LOADED()
	try
	{
		const std::string filepath = QFileDialog::getSaveFileName(this, "Save File", "", "MIZ Files (*.miz)").
			toStdString();
		if (filepath.empty())
			return;
		m_mission.SaveAs(filepath);
		LOG_TRACE("Mission {} saved sucessfully !", filepath);
		loadMission(filepath);
	} catch (const std::exception& except)
	{
		QMessageBox::critical(nullptr, "Error", except.what(), QMessageBox::Ok);
		LOG_ERROR(except.what());
	}
}

void MainWindow::refresh()
{
}

void MainWindow::manageScripts()
{
	CHECK_MISSION_LOADED()
	try
	{
		m_mission.ManageScripts();
		setButtons();
	} catch (const std::exception& except)
	{
		QMessageBox::critical(nullptr, "Error", except.what(), QMessageBox::Ok);
		LOG_ERROR(except.what());
	}
}

void MainWindow::updateScripts()
{
	CHECK_MISSION_LOADED()
	m_mission.UpdateScripts();
}

void MainWindow::help()
{
	QDesktopServices::openUrl(QUrl("https://github.com/JTFF-applications/Templateator2000"));
}

void MainWindow::about()
{
	AboutDialog dialog;
	dialog.exec();
}
#pragma endregion

#pragma region Buttons
void MainWindow::addTanker()
{
	CHECK_MISSION_LOADED()
	TankerWizard wizard(m_mission);
	wizard.exec();

	if (!wizard.IsDone())
		return;

	const auto& tk = wizard.GetTanker();
	m_mission.AddTanker(tk);
	m_ui.tankers->addItem(TANKER_PRESENTATION_STRING(tk).c_str());
	LOG_TRACE("Tanker {}-{} added !", tk.Callsign.Name, tk.Callsign.Number);
}

void MainWindow::addCarrier()
{
	CHECK_MISSION_LOADED()
	CarrierWindow win(
		nullptr,
		m_mission.GetMissionGroups(),
		[&](const Carrier& carrier)
		{
			m_mission.AddCarrier(carrier);
			m_ui.carrier->addItem(CARRIER_PRESENTATION_STRING(carrier).c_str());
			LOG_TRACE("Carrier {} added !", carrier.Alias);
		},
		[&]
		{
		});
	win.exec();
}

void MainWindow::addBeacon()
{
	CHECK_MISSION_LOADED()
	BeaconWizard wizard(m_mission);
	wizard.exec();

	if (!wizard.IsDone())
		return;

	const auto& beacon = wizard.GetBeacon();
	m_mission.AddBeacon(beacon);
	m_ui.beacon->addItem(BEACON_PRESENTATION_STRING(beacon).c_str());
	LOG_TRACE("Beacon {} added !", beacon.Name);
}

void MainWindow::addAwacs()
{
	CHECK_MISSION_LOADED()
	AwacsWizard wizard(m_mission);
	wizard.exec();

	if (!wizard.IsDone())
		return;

	const auto& awacs = wizard.GetAwacs();
	m_mission.AddAwacs(awacs);
	m_ui.awacs->addItem(AWACS_PRESENTATION_STRING(awacs).c_str());
	LOG_TRACE("Awacs {}-{} added !", awacs.Callsign.Name, awacs.Callsign.Number);
}

void MainWindow::addAtis()
{
	CHECK_MISSION_LOADED()
	AtisWizard wizard(m_mission);
	wizard.exec();

	if (!wizard.IsDone())
		return;

	const auto& atis = wizard.GetAtis();
	m_mission.AddAtis(atis);
	m_ui.atis->addItem(ATIS_PRESENTATION_STRING(atis).c_str());
	LOG_TRACE("Atis {} added !", atis.AirportName);
}

void MainWindow::removeTanker()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.tankers->selectedItems())
	{
		const std::string tanker_label = item->text().toStdString();
		const Tanker& tk = m_mission.GetTanker(tanker_label);
		m_mission.RemoveTanker(tanker_label);
		LOG_TRACE("Tanker {}-{} removed !", tk.Callsign.Name, tk.Callsign.Number);
	}
	qDeleteAll(m_ui.tankers->selectedItems());
}

void MainWindow::removeCarrier()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.carrier->selectedItems())
	{
		const std::string carrier_label = item->text().toStdString();
		const Carrier& carrier = m_mission.GetCarrier(carrier_label);
		m_mission.RemoveCarrier(carrier_label);
		LOG_TRACE("Carrier {} removed !", carrier.Alias);
	}
	qDeleteAll(m_ui.carrier->selectedItems());
}

void MainWindow::removeBeacon()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.beacon->selectedItems())
	{
		const std::string beacon_label = item->text().toStdString();
		const Beacon& beacon = m_mission.GetBeacon(beacon_label);
		m_mission.RemoveBeacon(beacon_label);
		LOG_TRACE("Beacon {} removed !", beacon.Name);
	}
	qDeleteAll(m_ui.beacon->selectedItems());
}

void MainWindow::removeAwacs()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.awacs->selectedItems())
	{
		const std::string awacs_label = item->text().toStdString();
		const Awacs& awacs = m_mission.GetAwacs(awacs_label);
		m_mission.RemoveAwacs(awacs_label);
		LOG_TRACE("Awacs {}-{} removed !", awacs.Callsign.Name, awacs.Callsign.Number);
	}
	qDeleteAll(m_ui.awacs->selectedItems());
}

void MainWindow::removeAtis()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.atis->selectedItems())
	{
		const std::string atis_label = item->text().toStdString();
		const Atis& atis = m_mission.GetAtis(atis_label);
		m_mission.RemoveAtis(atis_label);
		LOG_TRACE("Atis {} removed !", atis.AirportName);
	}
	qDeleteAll(m_ui.atis->selectedItems());
}

void MainWindow::editTanker()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.tankers->selectedItems())
	{
		const std::string tanker_label = item->text().toStdString();
		const auto& old_tk = m_mission.GetTanker(tanker_label);

		TankerWizard wizard(m_mission);
		wizard.SetTanker(old_tk);
		wizard.exec();

		if (!wizard.IsDone())
			return;

		const auto& new_tk = wizard.GetTanker();
		m_mission.ModifyTanker(old_tk, new_tk);
		item->setText(TANKER_PRESENTATION_STRING(new_tk).c_str());
		LOG_TRACE("Tanker {}-{} modified !", old_tk.Callsign.Name, old_tk.Callsign.Number);
	}
}

void MainWindow::editCarrier()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.carrier->selectedItems())
	{
		const std::string carrier_label = item->text().toStdString();
		const auto& old_carrier = m_mission.GetCarrier(carrier_label);
		CarrierWindow win(
			nullptr,
			m_mission.GetMissionGroups(),
			[&](const Carrier& new_carrier)
			{
				m_mission.ModifyCarrier(old_carrier, new_carrier);
				item->setText(CARRIER_PRESENTATION_STRING(new_carrier).c_str());
				LOG_TRACE("Carrier {} modified !", old_carrier.Alias);
			},
			[&]
			{
			});
		win.SetCarrier(old_carrier);
		win.exec();
	}
}

void MainWindow::editBeacon()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.beacon->selectedItems())
	{
		const std::string beacon_label = item->text().toStdString();
		const auto& old_bcn = m_mission.GetBeacon(beacon_label);

		BeaconWizard wizard(m_mission);
		wizard.SetBeacon(old_bcn);
		wizard.exec();

		if (!wizard.IsDone())
			return;

		const auto& new_bcn = wizard.GetBeacon();
		m_mission.ModifyBeacon(old_bcn, new_bcn);
		item->setText(BEACON_PRESENTATION_STRING(new_bcn).c_str());
		LOG_TRACE("Beacon {} modified !", old_bcn.Name);
	}
}

void MainWindow::editAwacs()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.awacs->selectedItems())
	{
		const std::string awacs_label = item->text().toStdString();
		const auto& old_awacs = m_mission.GetAwacs(awacs_label);

		AwacsWizard wizard(m_mission);
		wizard.SetAwacs(old_awacs);
		wizard.exec();

		if (!wizard.IsDone())
			return;

		const auto& new_awacs = wizard.GetAwacs();
		m_mission.ModifyAwacs(old_awacs, new_awacs);
		item->setText(AWACS_PRESENTATION_STRING(new_awacs).c_str());
		LOG_TRACE("Awacs {}-{} modified !", old_awacs.Callsign.Name, old_awacs.Callsign.Number);
	}
}

void MainWindow::editAtis()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.atis->selectedItems())
	{
		const std::string atis_label = item->text().toStdString();
		const auto& old_atis = m_mission.GetAtis(atis_label);

		AtisWizard wizard(m_mission);
		wizard.SetAtis(old_atis);
		wizard.exec();

		if (!wizard.IsDone())
			return;

		const auto& new_atis = wizard.GetAtis();
		m_mission.ModifyAtis(old_atis, new_atis);
		item->setText(ATIS_PRESENTATION_STRING(new_atis).c_str());
		LOG_TRACE("Atis {} modified !", old_atis.AirportName);
	}
}
#pragma endregion

#pragma region Fill UI
void MainWindow::fillTankers() const
{
	m_ui.tankers->clear();
	const auto& tankers = m_mission.GetScripts().GetTankers();
	for (const auto& tanker : tankers)
		m_ui.tankers->addItem(TANKER_PRESENTATION_STRING(tanker).c_str());
	LOG_TRACE("GetTankers filled !");
}

void MainWindow::fillAtis() const
{
	m_ui.atis->clear();
	const auto& atis_list = m_mission.GetScripts().GetAtis();
	for (const auto& atis : atis_list)
		m_ui.atis->addItem(ATIS_PRESENTATION_STRING(atis).c_str());
	LOG_TRACE("Atis filled !");
}

void MainWindow::fillBeacons() const
{
	m_ui.beacon->clear();
	const auto& beacons = m_mission.GetScripts().GetBeacons();
	for (const auto& beacon : beacons)
		m_ui.beacon->addItem(BEACON_PRESENTATION_STRING(beacon).c_str());
	LOG_TRACE("Atis filled !");
}

void MainWindow::fillCarriers() const
{
	m_ui.carrier->clear();
	const auto& carriers = m_mission.GetScripts().GetCarriers();
	for (const auto& carrier : carriers)
		m_ui.carrier->addItem(CARRIER_PRESENTATION_STRING(carrier).c_str());
	LOG_TRACE("Carriers filled !");
}

void MainWindow::fillAwacs() const
{
	m_ui.awacs->clear();
	const auto& awacs_list = m_mission.GetScripts().GetAwacs();
	for (const auto& awacs : awacs_list)
		m_ui.awacs->addItem(AWACS_PRESENTATION_STRING(awacs).c_str());
	LOG_TRACE("Awacs filled !");
}
#pragma endregion

void MainWindow::loadMission(const std::string& filepath)
{
	if (!DcsMission::IsValidMission(filepath))
	{
		QMessageBox::warning(this, "Error", "Invalid mission file !");
		LOG_WARN("Invalid mission {}", filepath);
		return;
	}

	try
	{
		const std::filesystem::path temp_filepath = std::format("temp/{}",
		                                                        std::filesystem::path(filepath).filename().string());
		copy_file(filepath, temp_filepath, std::filesystem::copy_options::overwrite_existing);
		m_mission.Init(temp_filepath, filepath);
		LOG_TRACE("Mission {} loaded sucessfully !", filepath);
	} catch (const std::exception& except)
	{
		QMessageBox::critical(nullptr, "Error", except.what(), QMessageBox::Ok);
		LOG_ERROR(except.what());
		return;
	}

	m_ui.selected_mission->setText(std::format("Selected mission : {}", m_mission.GetMissionName()).c_str());
	setButtons();

	fillTankers();
	fillAtis();
	fillBeacons();
	fillCarriers();
	fillAwacs();
}

void MainWindow::setButtonsState(const std::string& button_prefix, const bool& state) const
{
	findChild<QPushButton*>(std::format("{}_add_btn", button_prefix).c_str())->setEnabled(state);
	findChild<QPushButton*>(std::format("{}_rm_btn", button_prefix).c_str())->setEnabled(state);
	findChild<QPushButton*>(std::format("{}_edit_btn", button_prefix).c_str())->setEnabled(state);
}

void MainWindow::setButtons() const
{
	const std::vector names = {"tanker", "atis", "beacon", "carrier", "awacs"};
	for (const auto& name : names)
		setButtonsState(name, false);

	const auto& installed_scripts = m_mission.GetScripts().InstalledScripts();
	for (const std::string& name : installed_scripts)
		if (name == "tankers")
			setButtonsState("tanker", true);
		else if (name == "beacon")
			setButtonsState("beacon", true);
		else if (name == "airboss")
			setButtonsState("carrier", true);
		else if (std::ranges::find(names, name) != names.cend())
			setButtonsState(name, true);
}
