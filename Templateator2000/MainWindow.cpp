#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	connect(m_ui.actionNew, SIGNAL(triggered()), this, SLOT(New()));
	connect(m_ui.actionOpen, SIGNAL(triggered()), this, SLOT(Open()));
	connect(m_ui.actionSave, SIGNAL(triggered()), this, SLOT(Save()));
	connect(m_ui.actionSaveAs, SIGNAL(triggered()), this, SLOT(SaveAs()));
	connect(m_ui.actionExit, SIGNAL(triggered()), this, SLOT(Exit()));

	connect(m_ui.actionAddTanker, SIGNAL(triggered()), this, SLOT(AddTanker()));
	connect(m_ui.actionAddCarrier, SIGNAL(triggered()), this, SLOT(AddCarrier()));
	connect(m_ui.actionAddBeacon, SIGNAL(triggered()), this, SLOT(AddBeacon()));
	connect(m_ui.actionAddAtis, SIGNAL(triggered()), this, SLOT(AddAtis()));

	connect(m_ui.actionHelp, SIGNAL(triggered()), this, SLOT(Help()));
	connect(m_ui.actionAbout, SIGNAL(triggered()), this, SLOT(About()));
}

void MainWindow::New()
{
}

void MainWindow::Open()
{
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

void MainWindow::AddTanker()
{
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

void MainWindow::Help()
{
}

void MainWindow::About()
{
}
