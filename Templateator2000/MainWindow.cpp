#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>

#include "Utilities/Log.h"
#include "Windows/TankerWindow.h"
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

	connect(m_ui.actionAddTanker, SIGNAL(triggered()), this, SLOT(addTanker()));
	connect(m_ui.actionAddCarrier, SIGNAL(triggered()), this, SLOT(addCarrier()));
	connect(m_ui.actionAddBeacon, SIGNAL(triggered()), this, SLOT(addBeacon()));
	connect(m_ui.actionAddAtis, SIGNAL(triggered()), this, SLOT(addAtis()));

	connect(m_ui.actionHelp, SIGNAL(triggered()), this, SLOT(help()));
	connect(m_ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));

	connect(m_ui.tanker_add_btn, SIGNAL(clicked()), this, SLOT(addTanker()));
	connect(m_ui.carrier_add_btn, SIGNAL(clicked()), this, SLOT(addCarrier()));
	connect(m_ui.beacon_add_btn, SIGNAL(clicked()), this, SLOT(addBeacon()));
	connect(m_ui.atis_add_btn, SIGNAL(clicked()), this, SLOT(addAtis()));

	connect(m_ui.tanker_rm_btn, SIGNAL(clicked()), this, SLOT(removeTanker()));
	connect(m_ui.carrier_rm_btn, SIGNAL(clicked()), this, SLOT(removeCarrier()));
	connect(m_ui.beacon_rm_btn, SIGNAL(clicked()), this, SLOT(removeBeacon()));
	connect(m_ui.atis_rm_btn, SIGNAL(clicked()), this, SLOT(removeAtis()));

	connect(m_ui.tanker_edit_btn, SIGNAL(clicked()), this, SLOT(editTanker()));
	connect(m_ui.carrier_edit_btn, SIGNAL(clicked()), this, SLOT(editCarrier()));
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
	fillTankers();
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
	} catch (const std::exception& except)
	{
		QMessageBox::critical(nullptr, "Error", except.what(), QMessageBox::Ok);
		LOG_ERROR(except.what());
	}
}

void MainWindow::refresh()
{
}

void MainWindow::help()
{
}

void MainWindow::about()
{
}
#pragma endregion

#pragma region Buttons
void MainWindow::addTanker()
{
	CHECK_MISSION_LOADED()
	TankerWindow win(
		nullptr,
		m_mission.GetMissionGroups(),
		[&](const Tanker& tk)
		{
			m_mission.AddTanker(tk);
			m_ui.tankers->addItem(TANKER_PRESENTATION_STRING(tk).c_str());
			LOG_TRACE("Tanker {}-{} added !", tk.Callsign, tk.CallsignNb);
		},
		[&]
		{
		});
	win.exec();
}

void MainWindow::addCarrier()
{
}

void MainWindow::addBeacon()
{
}

void MainWindow::addAtis()
{
}

void MainWindow::removeTanker()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.tankers->selectedItems())
	{
		const std::string tanker_label = item->text().toStdString();
		const Tanker& tk = m_mission.GetTanker(tanker_label);
		m_mission.RemoveTanker(tanker_label);
		LOG_TRACE("Tanker {}-{} removed !", tk.Callsign, tk.CallsignNb);
	}
	qDeleteAll(m_ui.tankers->selectedItems());
}

void MainWindow::removeCarrier()
{
}

void MainWindow::removeBeacon()
{
}

void MainWindow::removeAtis()
{
}

void MainWindow::editTanker()
{
	CHECK_MISSION_LOADED()
	for (const auto& item : m_ui.tankers->selectedItems())
	{
		const std::string tanker_label = item->text().toStdString();
		const auto& old_tk = m_mission.GetTanker(tanker_label);
		TankerWindow win(
			nullptr,
			m_mission.GetMissionGroups(),
			[&](const Tanker& new_tk)
			{
				m_mission.ModifyTanker(old_tk, new_tk);
				item->setText(TANKER_PRESENTATION_STRING(new_tk).c_str());
				LOG_TRACE("Tanker {}-{} modified !", old_tk.Callsign, old_tk.CallsignNb);
			},
			[&]
			{
			});
		win.SetTanker(old_tk);
		win.exec();
	}
}

void MainWindow::editCarrier()
{
}

void MainWindow::editBeacon()
{
}

void MainWindow::editAtis()
{
}
#pragma endregion

#pragma region Fill UI
void MainWindow::fillTankers() const
{
	const auto& tankers = m_mission.GetScripts().Tankers();
	for (const auto& tanker : tankers)
		m_ui.tankers->addItem(TANKER_PRESENTATION_STRING(tanker).c_str());
	LOG_TRACE("Tankers filled !");
}
#pragma endregion
