#include "Windows/Wizards/Tanker/TankerWizard.h"

TankerWizard::TankerWizard(const Mission& mission, QWidget* parent)
	: QWizard(parent)
{
	m_intro_page = new TankerIntroPage(this);
	m_respawn_page = new TankerRespawnPage(this);
	m_model_page = new TankerModelPage(mission, this);
	m_departure_page = new TankerDeparturePage(mission, this);
	m_parking_page = new TankerParkingPage(this);
	m_carrier_page = new TankerCarrierPage(this);
	m_ondemand_flight_page = new TankerOnDemandFlightPage(mission, this);
	m_flight_page = new TankerFlightPage(this);
	m_mission_page = new TankerMissionPage(this);
	m_fixed_page = new TankerFixedPage(this);
	m_ondemand_page = new TankerOnDemandPage(this);
	m_escort_page = new TankerEscortPage(this);
	m_choose_escort_page = new TankerChooseEscortPage(mission, this);
	m_radio_page = new TankerRadioPage(mission, this);
	m_tacan_page = new TankerTacanPage(this);

	setPage(IntroPage, m_intro_page);
	setPage(RespawnPage, m_respawn_page);
	setPage(ModelPage, m_model_page);
	setPage(DeparturePage, m_departure_page);
	setPage(ParkingPage, m_parking_page);
	setPage(CarrierPage, m_carrier_page);
	setPage(OnDemandFlightPage, m_ondemand_flight_page);
	setPage(FlightPage, m_flight_page);
	setPage(MissionPage, m_mission_page);
	setPage(FixedPage, m_fixed_page);
	setPage(OnDemandPage, m_ondemand_page);
	setPage(EscortPage, m_escort_page);
	setPage(ChooseEscortPage, m_choose_escort_page);
	setPage(RadioPage, m_radio_page);
	setPage(TacanPage, m_tacan_page);

	connect(m_tacan_page, &TankerTacanPage::finished, this, &TankerWizard::finished);
	connect(m_model_page, &TankerModelPage::init, this, &TankerWizard::configureTankerModel);
	connect(m_departure_page, &TankerDeparturePage::init, this, &TankerWizard::configureTankerDeparture);
	connect(m_choose_escort_page, &TankerChooseEscortPage::init, this, &TankerWizard::configureTankerEscort);
}

Tanker TankerWizard::GetTanker() const
{
	if (!m_done)
		throw std::runtime_error("Can't call TankerWizard::GetTanker when wizard is not done !");
	return m_tanker;
}

void TankerWizard::SetTanker(const Tanker& tanker)
{
	m_modified = true;
	m_tanker = tanker;

	m_intro_page->m_fixed_tanker_btn->setChecked(tanker.Type == Tanker::Type::Fixed);
	m_intro_page->m_on_demand_tanker_btn->setChecked(tanker.Type == Tanker::Type::OnDemand);
	m_intro_page->m_coalition->setCurrentText(Coalition::ToString(tanker.Coalition).c_str());
	m_respawn_page->m_yes_btn->setChecked(tanker.AutoRespawn);
	m_respawn_page->m_no_btn->setChecked(!tanker.AutoRespawn);
	m_carrier_page->m_airboss_yes->setChecked(tanker.AirbossRecovery);
	m_departure_page->m_departure_list->setCurrentText(tanker.DepartureBase.c_str());
	m_parking_page->m_parkings_list->setCurrentText(tanker.TerminalType.c_str());
	m_model_page->m_group_list->setCurrentText(tanker.GroupName.c_str());
	m_escort_page->m_escort_no->setChecked(tanker.EscortGroupName.empty());
	m_choose_escort_page->m_escort_list->setCurrentText(tanker.EscortGroupName.c_str());
	m_radio_page->m_frequency->setText(tanker.Frequency.c_str());
	m_mission_page->m_duration->setText(QString::number(tanker.MaxMissionDuration));
	m_mission_page->m_fuel_level->setText(QString::number(tanker.FuelWarningLevel));
	m_carrier_page->m_modex->setText(QString::number(tanker.Modex));

	m_tacan_page->m_channel->setText(QString::number(tanker.Tacan.Channel));
	m_tacan_page->m_band->setCurrentText(tanker.Tacan.Band.c_str());
	m_tacan_page->m_morse->setText(tanker.Tacan.Morse.c_str());

	m_radio_page->m_callsign_list->setCurrentText(tanker.Callsign.Name.c_str());
	m_radio_page->m_callsign_number->setText(QString::number(tanker.Callsign.Number));

	m_model_page->m_pattern_list->setCurrentText(tanker.PatternUnit.c_str());
	m_flight_page->m_altitude->setText(QString::number(tanker.Altitude));
	m_flight_page->m_speed->setText(QString::number(tanker.Speed));
	m_fixed_page->m_racetrack_front->setText(QString::number(tanker.Racetrack.Front));
	m_fixed_page->m_racetrack_back->setText(QString::number(tanker.Racetrack.Back));

	m_ondemand_flight_page->m_name->setText(tanker.Name.c_str());
	m_ondemand_flight_page->m_yes_btn->setChecked(tanker.Type == Tanker::Type::OnDemand && tanker.Altitude != 0);
	m_ondemand_flight_page->m_no_btn->setChecked(!(tanker.Type == Tanker::Type::OnDemand && tanker.Altitude != 0));

	m_ondemand_page->m_orbit_heading->setText(QString::number(tanker.Orbit.Heading));
	m_ondemand_page->m_orbit_length->setText(QString::number(tanker.Orbit.Length));
}

bool TankerWizard::IsCustomValues() const
{
	return m_ondemand_flight_page->m_yes_btn->isChecked();
}

Coalition::Side TankerWizard::Coalition() const
{
	return Coalition::FromString(m_intro_page->m_coalition->currentText().toStdString());
}

enum Tanker::Type TankerWizard::Type() const
{
	return m_intro_page->m_fixed_tanker_btn->isChecked() ? Tanker::Type::Fixed : Tanker::Type::OnDemand;
}

void TankerWizard::configureTankerModel() const
{
	if (!m_modified)
		return;
	m_model_page->m_pattern_list->setCurrentText(m_tanker.PatternUnit.c_str());
	m_model_page->m_group_list->setCurrentText(m_tanker.GroupName.c_str());
}

void TankerWizard::configureTankerDeparture() const
{
	if (!m_modified)
		return;
	m_departure_page->m_departure_list->setCurrentText(m_tanker.DepartureBase.c_str());
}

void TankerWizard::configureTankerEscort() const
{
	if (!m_modified)
		return;
	m_choose_escort_page->m_escort_list->setCurrentText(m_tanker.EscortGroupName.c_str());
}

void TankerWizard::finished()
{
	const bool is_escorted = m_escort_page->m_escort_yes->isChecked();
	const enum Tanker::Type type = Type();

	m_tanker = {
		.Type = type,
		.Coalition = Coalition::FromString(m_intro_page->m_coalition->currentText().toStdString()),
		.AutoRespawn = m_respawn_page->m_yes_btn->isChecked(),
		.AirbossRecovery = m_carrier_page->m_airboss_yes->isChecked(),
		.DepartureBase = m_departure_page->m_departure_list->currentText().toStdString(),
		.TerminalType = m_parking_page->m_parkings_list->currentText().toStdString(),
		.GroupName = m_model_page->m_group_list->currentText().toStdString(),
		.EscortGroupName = is_escorted ? m_choose_escort_page->m_escort_list->currentText().toStdString() : "",
		.Frequency = m_radio_page->m_frequency->text().toStdString(),
		.MaxMissionDuration = m_mission_page->m_duration->text().toInt(),
		.FuelWarningLevel = m_mission_page->m_fuel_level->text().toInt(),
		.Modex = m_carrier_page->m_modex->text().toInt(),
		.Tacan = {
			.Channel = m_tacan_page->m_channel->text().toInt(),
			.Band = m_tacan_page->m_band->currentText().toStdString(),
			.Morse = m_tacan_page->m_morse->text().toStdString()
		},
		.Callsign = {
			.Name = m_radio_page->m_callsign_list->currentText().toStdString(),
			.Number = m_radio_page->m_callsign_number->text().toInt()
		},
		.PatternUnit = m_model_page->m_pattern_list->currentText().toStdString(),
		.Altitude = m_flight_page->m_altitude->text().toInt(),
		.Speed = m_flight_page->m_speed->text().toInt(),
		.Racetrack = {
			.Front = m_fixed_page->m_racetrack_front->text().toInt(),
			.Back = m_fixed_page->m_racetrack_back->text().toInt()
		},
		.Name = m_ondemand_flight_page->m_name->text().toStdString(),
		.Orbit = {
			.Heading = m_ondemand_page->m_orbit_heading->text().toInt(),
			.Length = m_ondemand_page->m_orbit_length->text().toInt()
		}
	};

	m_done = true;
}
