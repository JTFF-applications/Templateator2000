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

	virtual void closeEvent(QCloseEvent* event) override;

private slots:
	// Top Bar
	void open();
	void save();
	void saveAs();

	void refresh();
	void manageScripts();

	static void help();
	void about();

	// Buttons
	void addTanker();
	void addCarrier();
	void addBeacon();
	void addAwacs();
	void addAtis();

	void removeTanker();
	void removeCarrier();
	void removeBeacon();
	void removeAwacs();
	void removeAtis();

	void editTanker();
	void editCarrier();
	void editBeacon();
	void editAwacs();
	void editAtis();

private:
	void fillTankers() const;
	void fillAtis() const;
	void fillBeacons() const;
	void fillCarriers() const;
	void fillAwacs() const;

private:
	Ui::MainWindowClass m_ui {};
	Mission m_mission;
};
