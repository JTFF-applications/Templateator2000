#include <QCompleter>

#include "Utilities/Mission.h"
#include "Utilities/Validators/QStringListValidator.h"

#include "Windows/BeaconWindow.h"

BeaconWindow::BeaconWindow(QWidget* parent,
                           const std::map<const std::string, const std::vector<Group>>& mission_data,
                           const std::function<void(Beacon)>& on_ok,
                           const std::function<void()>& on_cancel)
	: QDialog(parent), m_mission_data(mission_data), m_on_ok(on_ok), m_on_cancel(on_cancel)
{
	m_ui.setupUi(this);

	m_ui.unit_name->setCompleter(new QCompleter(Mission::DataToUnitName(m_mission_data)));

	m_ui.unit_name->setValidator(new QStringListValidator(Mission::DataToUnitName(m_mission_data)));
	m_ui.tacan_morse->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Z]{3}$")));

	connect(m_ui.ok_btn, &QPushButton::clicked, this, &BeaconWindow::onOkClicked);
	connect(m_ui.cancel_btn, &QPushButton::clicked, this, &BeaconWindow::onCancelClicked);
}

BeaconWindow::~BeaconWindow()
{
	delete m_ui.unit_name->completer();

	delete m_ui.unit_name->validator();
	delete m_ui.tacan_morse->validator();
}

void BeaconWindow::SetBeacon(const Beacon& beacon) const
{
	m_ui.name->setText(beacon.Name.c_str());
	m_ui.unit_name->setText(beacon.UnitName.c_str());
	m_ui.tacan->setValue(beacon.Tacan.Channel);
	m_ui.tacan_band->setCurrentIndex(m_ui.tacan_band->findData(beacon.Tacan.Band.c_str()));
	m_ui.tacan_morse->setText(beacon.Tacan.Morse.c_str());
}

void BeaconWindow::onOkClicked()
{
	try
	{
		Beacon beacon = {
			.Name = m_ui.name->text().toStdString(),
			.UnitName = m_ui.unit_name->text().toStdString(),
			.Tacan = {
				.Channel = m_ui.tacan->text().toInt(),
				.Band = m_ui.tacan_band->currentText().toStdString(),
				.Morse = m_ui.tacan_morse->text().toStdString()
			}
		};

		QString unit_name = beacon.UnitName.c_str();
		int nb = 0;

		if (beacon.Name.empty())
			throw std::exception("Name is empty !");
		if (beacon.UnitName.empty() || m_ui.unit_name->validator()->validate(unit_name, nb) == QValidator::Invalid)
			throw std::exception("Invalid unit name !");
		if (beacon.Tacan.Morse.empty())
			throw std::exception("Invalid tacan parameters !");

		close();
		std::invoke(m_on_ok, beacon);
	} catch (const std::exception& except)
	{
		m_ui.error_msg->setText(except.what());
		LOG_ERROR(except.what());
	}
}

void BeaconWindow::onCancelClicked()
{
	close();
	std::invoke(m_on_cancel);
}
