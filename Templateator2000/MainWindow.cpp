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

	connect(m_ui.actionExport, SIGNAL(triggered()), this, SLOT(Export()));
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
}

#pragma region TopBar
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

void MainWindow::Export()
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

void MainWindow::ModifyTanker()
{
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
