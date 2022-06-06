#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

private slots:
	void New();
	void Open();
	void Save();
	void SaveAs();
	void Exit();

	void Refresh();

	void AddTanker();
	void AddCarrier();
	void AddBeacon();
	void AddAtis();

	void Help();
	void About();
private:
	Ui::MainWindowClass m_ui;
};
