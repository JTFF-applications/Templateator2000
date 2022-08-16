#include "Windows/Wizards/Beacon/BeaconWizard.h"

BeaconWizard::BeaconWizard(const Mission& mission, QWidget* parent)
	: QWizard(parent)
{
	m_intro_page = new BeaconIntroPage(this);
	m_info_page = new BeaconInfoPage(mission, this);
	m_tacan_page = new BeaconTacanPage(this);

	setPage(IntroPage, m_intro_page);
	setPage(InfoPage, m_info_page);
	setPage(TacanPage, m_tacan_page);

	connect(m_tacan_page, &BeaconTacanPage::finished, this, &BeaconWizard::finished);
}

Beacon BeaconWizard::GetBeacon() const
{
	if (!m_done)
		throw std::runtime_error("Can't call BeaconWizard::GetBeacon when wizard is not done !");
	return m_beacon;
}

void BeaconWizard::SetBeacon(const Beacon& beacon)
{
	m_modified = true;
	m_beacon = beacon;

	m_info_page->m_name->setText(beacon.Name.c_str());
	m_info_page->m_unit->setCurrentText(beacon.UnitName.c_str());

	m_tacan_page->m_channel->setText(QString::number(beacon.Tacan.Channel));
	m_tacan_page->m_band->setCurrentText(beacon.Tacan.Band.c_str());
	m_tacan_page->m_morse->setText(beacon.Tacan.Morse.c_str());
}

void BeaconWizard::finished()
{
	m_beacon = {
		.Name = m_info_page->m_name->text().toStdString(),
		.UnitName = m_info_page->m_unit->currentText().toStdString(),
		.Tacan = {
			.Channel = m_tacan_page->m_channel->text().toInt(),
			.Band = m_tacan_page->m_band->currentText().toStdString(),
			.Morse = m_tacan_page->m_morse->text().toStdString()
		}
	};

	m_done = true;
}
