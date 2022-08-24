#include "Windows/Wizards/Atis/AtisWizard.h"

AtisWizard::AtisWizard(const Mission& mission, QWidget* parent)
	: QWizard(parent)
{
	m_intro_page = new AtisIntroPage(mission, this);
	m_general_page = new AtisGeneralPage(mission, this);
	m_settings_page = new AtisSettingsPage(this);
	m_runway_page = new AtisRunwayPage(this);
	m_radio_page = new AtisRadioPage(this);
	m_tacan_page = new AtisTacanPage(this);
	m_ils_page = new AtisIlsPage(this);
	m_srs_page = new AtisSrsPage(this);

	setPage(IntroPage, m_intro_page);
	setPage(GeneralPage, m_general_page);
	setPage(SettingsPage, m_settings_page);
	setPage(RunwayPage, m_runway_page);
	setPage(RadioPage, m_radio_page);
	setPage(TacanPage, m_tacan_page);
	setPage(IlsPage, m_ils_page);
	setPage(SrsPage, m_srs_page);

	connect(m_srs_page, &AtisSrsPage::finished, this, &AtisWizard::finished);
}

Atis AtisWizard::GetAtis() const
{
	if (!m_done)
		throw std::runtime_error("Can't call AtisWizard::GetAtis when wizard not done !");
	return m_atis;
}

void AtisWizard::SetAtis(const Atis& atis)
{
	m_modified = true;
	m_atis = atis;

	const bool has_runway = !atis.ActiveRunwayNumber.empty();
	const bool has_radio = !atis.Radio.TowerFrequencies.empty();
	const bool has_ils = !atis.Ils.Frequency.empty();
	const bool has_tacan = !atis.Tacan.Morse.empty();

	m_intro_page->m_airport->setCurrentText(atis.AirportName.c_str());
	m_intro_page->m_frequency->setText(atis.Radio.Frequency.c_str());

	m_general_page->m_relay_unit->setCurrentText(atis.Radio.RelayUnit.c_str());
	m_general_page->m_modulation->setCurrentText(atis.Radio.Modulation.c_str());
	m_general_page->m_power->setText(QString::number(atis.Radio.Power));

	m_settings_page->m_runway->setChecked(has_runway);
	m_settings_page->m_radio->setChecked(has_radio);
	m_settings_page->m_ils->setChecked(has_ils);
	m_settings_page->m_tacan->setChecked(has_tacan);

	m_runway_page->m_runway->setText(std::format("{}{}", atis.ActiveRunwayNumber, atis.ActiveRunwaySide).c_str());
	for (const auto& fq : atis.Radio.TowerFrequencies)
		m_radio_page->m_frequencies->addItem(fq.c_str());
	m_ils_page->m_frequency->setText(atis.Ils.Frequency.c_str());
	m_ils_page->m_runway->setText(atis.Ils.Runway.c_str());
	m_tacan_page->m_channel->setText(QString::number(atis.Tacan.Channel));
	m_tacan_page->m_band->setCurrentText(atis.Tacan.Band.c_str());
	m_tacan_page->m_morse->setText(atis.Tacan.Morse.c_str());

	m_srs_page->m_srs_path->setText(atis.SrsPath.c_str());
}

int AtisWizard::NextOption(const int& current_page) const
{
	std::list<int> options;
	if (m_settings_page->m_runway->isChecked())
		options.push_back(RunwayPage);
	if (m_settings_page->m_radio->isChecked())
		options.push_back(RadioPage);
	if (m_settings_page->m_ils->isChecked())
		options.push_back(IlsPage);
	if (m_settings_page->m_tacan->isChecked())
		options.push_back(TacanPage);

	auto next_option = std::ranges::find(options, current_page);
	if (++next_option == options.end())
		return SrsPage;
	return *next_option;
}

void AtisWizard::finished()
{
	const std::string runway = m_runway_page->m_runway->text().toStdString();

	m_atis = {
		.AirportName = m_intro_page->m_airport->currentText().toStdString(),
		.ActiveRunwayNumber = !runway.empty() ? runway.substr(0, 2) : "",
		.ActiveRunwaySide = !runway.empty() ? std::isdigit(runway.back()) ? "" : std::string(runway.back(), 1) : "",
		.Radio = {
			.Frequency = m_intro_page->m_frequency->text().toStdString(),
			.Modulation = m_general_page->m_modulation->currentText().toStdString(),
			.RelayUnit = m_general_page->m_relay_unit->currentText().toStdString(),
			.TowerFrequencies = m_radio_page->Frequencies(),
			.Power = m_general_page->m_power->text().toInt()
		},
		.Tacan = {
			.Channel = m_tacan_page->m_channel->text().toInt(),
			.Band = m_tacan_page->m_band->currentText().toStdString(),
			.Morse = m_tacan_page->m_morse->text().toStdString()
		},
		.Ils = {
			.Frequency = m_ils_page->m_frequency->text().toStdString(),
			.Runway = m_ils_page->m_runway->text().toStdString()
		},
		.SrsPath = m_srs_page->m_srs_path->text().toStdString()
	};
	m_done = true;
}
