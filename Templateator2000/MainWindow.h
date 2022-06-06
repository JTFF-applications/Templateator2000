#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

private slots:
	// Top Bar
	void New();
	void Open();
	void Save();
	void SaveAs();
	void Exit();

	void Refresh();
	void Export();

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

	void ModifyTanker();
	void EditCarrier();
	void EditBeacon();
	void EditAtis();
private:
	Ui::MainWindowClass m_ui;
};
