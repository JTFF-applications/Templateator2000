#include <QFileDialog>
#include <QMessageBox>

#include "Utilities/Log.h"
#include "Windows/TankerWindow.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	connect(m_ui.actionOpen, SIGNAL(triggered()), this, SLOT(Open()));
	connect(m_ui.actionSave, SIGNAL(triggered()), this, SLOT(Save()));
	connect(m_ui.actionSaveAs, SIGNAL(triggered()), this, SLOT(SaveAs()));
	connect(m_ui.actionExit, SIGNAL(triggered()), this, SLOT(Exit()));

	connect(m_ui.actionRefresh, SIGNAL(triggered()), this, SLOT(Refresh()));

	connect(m_ui.actionAddTanker, SIGNAL(triggered()), this, SLOT(AddTanker()));
	connect(m_ui.actionAddCarrier, SIGNAL(triggered()), this, SLOT(AddCarrier()));
	connect(m_ui.actionAddBeacon, SIGNAL(triggered()), this, SLOT(AddBeacon()));
	connect(m_ui.actionAddAtis, SIGNAL(triggered()), this, SLOT(AddAtis()));

	connect(m_ui.actionHelp, SIGNAL(triggered()), this, SLOT(Help()));
	connect(m_ui.actionAbout, SIGNAL(triggered()), this, SLOT(About()));

	connect(m_ui.tanker_add_btn, SIGNAL(clicked()), this, SLOT(AddTanker()));
	connect(m_ui.carrier_add_btn, SIGNAL(clicked()), this, SLOT(AddCarrier()));
	connect(m_ui.beacon_add_btn, SIGNAL(clicked()), this, SLOT(AddBeacon()));
	connect(m_ui.atis_add_btn, SIGNAL(clicked()), this, SLOT(AddAtis()));

	connect(m_ui.tanker_rm_btn, SIGNAL(clicked()), this, SLOT(RemoveTanker()));
	connect(m_ui.carrier_rm_btn, SIGNAL(clicked()), this, SLOT(RemoveCarrier()));
	connect(m_ui.beacon_rm_btn, SIGNAL(clicked()), this, SLOT(RemoveBeacon()));
	connect(m_ui.atis_rm_btn, SIGNAL(clicked()), this, SLOT(RemoveAtis()));

	connect(m_ui.tanker_edit_btn, SIGNAL(clicked()), this, SLOT(EditTanker()));
	connect(m_ui.carrier_edit_btn, SIGNAL(clicked()), this, SLOT(EditCarrier()));
	connect(m_ui.beacon_edit_btn, SIGNAL(clicked()), this, SLOT(EditBeacon()));
	connect(m_ui.atis_edit_btn, SIGNAL(clicked()), this, SLOT(EditAtis()));

	if (!std::filesystem::exists("temp"))
		std::filesystem::create_directory("temp");
}

MainWindow::~MainWindow()
{
	if (std::filesystem::exists("temp"))
		std::filesystem::remove_all("temp");
}

#pragma region TopBar
void MainWindow::Open()
{
	const std::string filepath = QFileDialog::getOpenFileName(this, "Open File", "", "MIZ Files (*.miz)").toStdString();
	if (filepath.empty())
		return;

	if (!DCSMission::IsValidMission(filepath))
	{
		QMessageBox::warning(this, "Error", "Invalid mission file !");
		LOG_WARN("Invalid mission {}", filepath);
		return;
	}

	try
	{
		std::filesystem::path temp_filepath = std::format("temp/{}", std::filesystem::path(filepath).filename().string());
		std::filesystem::copy_file(filepath, temp_filepath, std::filesystem::copy_options::overwrite_existing);
		m_mission.Init(temp_filepath);
		LOG_TRACE("Mission {} loaded sucessfully !", filepath);
	}
	catch (const std::exception& except)
	{
		QMessageBox::critical(nullptr, "Error", except.what(), QMessageBox::Ok);
		LOG_ERROR(except.what());
		return;
	}

	m_ui.selected_mission->setText(std::format("Selected mission : {}", m_mission.GetMissionName()).c_str());
	FillTankers();
}

void MainWindow::Save()
{
}

void MainWindow::SaveAs()
{
}

void MainWindow::Exit()
{
}

void MainWindow::Refresh()
{
}

void MainWindow::Help()
{
}

void MainWindow::About()
{
}
#pragma endregion

#pragma region Buttons
void MainWindow::AddTanker()
{
	CHECK_MISSION_LOADED();
	TankerWindow win(
		nullptr,
		m_mission.GetMissionGroups(),
		[&](const Tanker& tk) {
			m_mission.AddTanker(tk);
			LOG_TRACE("Tanker {}-{} added !", tk.Callsign, tk.CallsignNb);
		},
		[&] {});
	win.exec();
}

void MainWindow::AddCarrier()
{
}

void MainWindow::AddBeacon()
{
}

void MainWindow::AddAtis()
{
}

void MainWindow::RemoveTanker()
{
	CHECK_MISSION_LOADED();
	for (auto& item : m_ui.tankers->selectedItems())
	{
		const std::string tanker_label = item->text().toStdString();
		const Tanker& tk = m_mission.GetTanker(tanker_label);
		m_mission.RemoveTanker(tanker_label);
		LOG_TRACE("Tanker {}-{} removed !", tk.Callsign, tk.CallsignNb);
	}
	qDeleteAll(m_ui.tankers->selectedItems());
}

void MainWindow::RemoveCarrier()
{
}

void MainWindow::RemoveBeacon()
{
}

void MainWindow::RemoveAtis()
{
}

void MainWindow::EditTanker()
{
	CHECK_MISSION_LOADED();
	for (const auto& item : m_ui.tankers->selectedItems())
	{
		const std::string tanker_label = item->text().toStdString();
		const auto& old_tk = m_mission.GetTanker(tanker_label);
		TankerWindow win(
			nullptr,
			m_mission.GetMissionGroups(),
			[&](const Tanker& new_tk) {
				m_mission.ModifyTanker(old_tk, new_tk);
				item->setText(TANKER_PRESENTATION_STRING(new_tk).c_str());
				LOG_TRACE("Tanker {}-{} modified !", old_tk.Callsign, old_tk.CallsignNb);
			},
			[&] {});
		win.SetTanker(old_tk);
		win.exec();
	}
}

void MainWindow::EditCarrier()
{
}

void MainWindow::EditBeacon()
{
}

void MainWindow::EditAtis()
{
}
#pragma endregion

#pragma region Fill UI
void MainWindow::FillTankers()
{
	const auto& tankers = m_mission.GetScripts().Tankers();
	for (const auto& tanker : tankers)
		m_ui.tankers->addItem(TANKER_PRESENTATION_STRING(tanker).c_str());
	LOG_TRACE("Tankers filled !");
}
#pragma endregion
