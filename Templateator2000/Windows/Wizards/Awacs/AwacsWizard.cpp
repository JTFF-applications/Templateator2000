#include "Windows/Wizards/Awacs/AwacsWizard.h"

AwacsWizard::AwacsWizard(const Mission& mission, QWidget* parent)
	: QWizard(parent)
{
	m_intro_page = new AwacsIntroPage(this);
	m_respawn_page = new AwacsRespawnPage(this);
	m_model_page = new AwacsModelPage(mission, this);
	m_departure_page = new AwacsDeparturePage(mission, this);
	m_parking_page = new AwacsParkingPage(this);
	m_carrier_page = new AwacsCarrierPage(this);
	m_flight_page = new AwacsFlightPage(this);
	m_mission_page = new AwacsMissionPage(this);
	m_fixed_page = new AwacsFixedPage(this);
	m_escort_page = new AwacsEscortPage(this);
	m_choose_escort_page = new AwacsChooseEscortPage(mission, this);
	m_radio_page = new AwacsRadioPage(mission, this);
	m_tacan_page = new AwacsTacanPage(this);

	setPage(IntroPage, m_intro_page);
	setPage(RespawnPage, m_respawn_page);
	setPage(ModelPage, m_model_page);
	setPage(DeparturePage, m_departure_page);
	setPage(ParkingPage, m_parking_page);
	setPage(CarrierPage, m_carrier_page);
	setPage(FlightPage, m_flight_page);
	setPage(MissionPage, m_mission_page);
	setPage(FixedPage, m_fixed_page);
	setPage(EscortPage, m_escort_page);
	setPage(ChooseEscortPage, m_choose_escort_page);
	setPage(RadioPage, m_radio_page);
	setPage(TacanPage, m_tacan_page);

	connect(m_tacan_page, &AwacsTacanPage::finished, this, &AwacsWizard::finished);
	connect(m_model_page, &AwacsModelPage::init, this, &AwacsWizard::configureAwacsModel);
	connect(m_departure_page, &AwacsDeparturePage::init, this, &AwacsWizard::configureAwacsDeparture);
	connect(m_choose_escort_page, &AwacsChooseEscortPage::init, this, &AwacsWizard::configureAwacsEscort);
}

Awacs AwacsWizard::GetAwacs() const
{
	if (!m_done)
		throw std::runtime_error("Can't call AwacsWizard::GetAwacs when wizard is not done !");
	return m_awacs;
}

void AwacsWizard::SetAwacs(const Awacs& awacs)
{
	m_modified = true;
	m_awacs = awacs;

	m_intro_page->m_coalition->setCurrentText(Coalition::ToString(awacs.Coalition).c_str());
	m_respawn_page->m_yes_btn->setChecked(awacs.AutoRespawn);
	m_respawn_page->m_no_btn->setChecked(!awacs.AutoRespawn);
	m_carrier_page->m_airboss_yes->setChecked(awacs.AirbossRecovery);
	m_departure_page->m_departure_list->setCurrentText(awacs.DepartureBase.c_str());
	m_parking_page->m_parkings_list->setCurrentText(awacs.TerminalType.c_str());
	m_model_page->m_group_list->setCurrentText(awacs.GroupName.c_str());
	m_escort_page->m_escort_no->setChecked(awacs.EscortGroupName.empty());
	m_choose_escort_page->m_escort_list->setCurrentText(awacs.EscortGroupName.c_str());
	m_radio_page->m_frequency->setText(awacs.Frequency.c_str());
	m_mission_page->m_duration->setText(QString::number(awacs.MaxMissionDuration));
	m_mission_page->m_fuel_level->setText(QString::number(awacs.FuelWarningLevel));
	m_carrier_page->m_modex->setText(QString::number(awacs.Modex));

	m_tacan_page->m_channel->setText(QString::number(awacs.Tacan.Channel));
	m_tacan_page->m_band->setCurrentText(awacs.Tacan.Band.c_str());
	m_tacan_page->m_morse->setText(awacs.Tacan.Morse.c_str());

	m_radio_page->m_callsign_list->setCurrentText(awacs.Callsign.Name.c_str());
	m_radio_page->m_callsign_number->setText(QString::number(awacs.Callsign.Number));

	m_model_page->m_pattern_list->setCurrentText(awacs.PatternUnit.c_str());
	m_flight_page->m_altitude->setText(QString::number(awacs.Altitude));
	m_flight_page->m_speed->setText(QString::number(awacs.Speed));
	m_fixed_page->m_racetrack_front->setText(QString::number(awacs.Racetrack.Front));
	m_fixed_page->m_racetrack_back->setText(QString::number(awacs.Racetrack.Back));
}

Coalition::Side AwacsWizard::Coalition() const
{
	return Coalition::FromString(m_intro_page->m_coalition->currentText().toStdString());
}

void AwacsWizard::configureAwacsModel() const
{
	if (!m_modified)
		return;
	m_model_page->m_pattern_list->setCurrentText(m_awacs.PatternUnit.c_str());
	m_model_page->m_group_list->setCurrentText(m_awacs.GroupName.c_str());
}

void AwacsWizard::configureAwacsDeparture() const
{
	if (!m_modified)
		return;
	m_departure_page->m_departure_list->setCurrentText(m_awacs.DepartureBase.c_str());
}

void AwacsWizard::configureAwacsEscort() const
{
	if (!m_modified)
		return;
	m_choose_escort_page->m_escort_list->setCurrentText(m_awacs.EscortGroupName.c_str());
}

void AwacsWizard::finished()
{
	const bool is_escorted = m_escort_page->m_escort_yes->isChecked();

	m_awacs = {
		.Coalition = Coalition::FromString(m_intro_page->m_coalition->currentText().toStdString()),
		.AutoRespawn = m_respawn_page->m_yes_btn->isChecked(),
		.AirbossRecovery = m_carrier_page->m_airboss_yes->isChecked(),
		.PatternUnit = m_model_page->m_pattern_list->currentText().toStdString(),
		.DepartureBase = m_departure_page->m_departure_list->currentText().toStdString(),
		.TerminalType = m_parking_page->m_parkings_list->currentText().toStdString(),
		.GroupName = m_model_page->m_group_list->currentText().toStdString(),
		.EscortGroupName = is_escorted ? m_choose_escort_page->m_escort_list->currentText().toStdString() : "",
		.Frequency = m_radio_page->m_frequency->text().toStdString(),
		.MaxMissionDuration = m_mission_page->m_duration->text().toInt(),
		.Altitude = m_flight_page->m_altitude->text().toInt(),
		.Speed = m_flight_page->m_speed->text().toInt(),
		.FuelWarningLevel = m_mission_page->m_fuel_level->text().toInt(),
		.Modex = m_carrier_page->m_modex->text().toInt(),
		.Tacan = {
			.Channel = m_tacan_page->m_channel->text().toInt(),
			.Band = m_tacan_page->m_band->currentText().toStdString(),
			.Morse = m_tacan_page->m_morse->text().toStdString()
		},
		.Racetrack = {
			.Front = m_fixed_page->m_racetrack_front->text().toInt(),
			.Back = m_fixed_page->m_racetrack_back->text().toInt()
		},
		.Callsign = {
			.Name = m_radio_page->m_callsign_list->currentText().toStdString(),
			.Number = m_radio_page->m_callsign_number->text().toInt()
		},
	};

	m_done = true;
}
