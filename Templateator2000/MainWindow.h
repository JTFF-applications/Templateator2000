#pragma once

#include "Utilities/Mission.h"
#include "ui_MainWindow.h"

#define CHECK_MISSION_LOADED()	if (!m_mission.IsInitialized()) { \
										QMessageBox::warning(this, "Error", "No mission loaded !");\
										LOG_WARN("No mission loaded");\
										return; \
									}

class MainWindow final : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	virtual ~MainWindow() override;

	MainWindow(const MainWindow&) = delete;
	MainWindow(const MainWindow&&) = delete;
	MainWindow& operator=(const MainWindow& other) = delete;
	MainWindow& operator=(const MainWindow&& other) = delete;

private slots:
	// Top Bar
	void Open();
	void Save();
	void SaveAs();
	void Exit();

	void Refresh();

	void Help();
	void About();

	// Buttons
	void AddTanker();
	void AddCarrier();
	void AddBeacon();
	void AddAtis();

	void RemoveTanker();
	void RemoveCarrier();
	void RemoveBeacon();
	void RemoveAtis();

	void EditTanker();
	void EditCarrier();
	void EditBeacon();
	void EditAtis();

private:
	void FillTankers() const;

private:
	Ui::MainWindowClass m_ui {};
	Mission m_mission;
};
