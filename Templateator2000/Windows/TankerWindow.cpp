#include "Utilities/Moose.h"

#include "TankerWindow.h"

TankerWindow::TankerWindow(QWidget* parent, const std::vector<Group>& groups, std::function<void(Tanker)> on_ok, std::function<void(void)> on_cancel)
	: QDialog(parent), m_groups(groups), m_onOk(on_ok), m_onCancel(on_cancel)
{
	m_ui.setupUi(this);

	m_ui.departure->addItems(Moose::GetQTAirbases());
	m_ui.parking_size->addItems(Moose::GetQTParkings());
	m_ui.callsign->addItems(Moose::GetQTTankerCallsigns());

	connect(m_ui.ok_btn, SIGNAL(clicked()), this, SLOT(on_ok_clicked));
	QObject::connect(m_ui.cancel_btn, &QPushButton::clicked, m_onCancel);
}

TankerWindow::TankerWindow(QWidget* parent, const Tanker& tanker, const std::vector<Group>& groups, std::function<void(Tanker)> on_ok, std::function<void(void)> on_cancel)
	: QDialog(parent), m_groups(groups), m_onOk(on_ok), m_onCancel(on_cancel)
{
	m_ui.setupUi(this);

	m_ui.departure->addItems(Moose::GetQTAirbases());
	m_ui.parking_size->addItems(Moose::GetQTParkings());
	m_ui.callsign->addItems(Moose::GetQTTankerCallsigns());

	// FIXME : Fill UI

	connect(m_ui.ok_btn, SIGNAL(clicked()), this, SLOT(on_ok_clicked));
	QObject::connect(m_ui.cancel_btn, &QPushButton::clicked, m_onCancel);
}

void TankerWindow::on_ok_clicked()
{
	Tanker tanker;
	// FIXME : Fill tanker
	// FIXME : Validate tanker
	std::invoke(m_onOk, tanker);
}
