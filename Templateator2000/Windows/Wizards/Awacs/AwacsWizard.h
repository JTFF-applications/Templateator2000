#pragma once

#include "Models/Awacs.h"
#include "Utilities/Mission.h"
#include "Windows/Wizards/Awacs/Pages/AwacsCarrierPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsChooseEscortPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsDeparturePage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsEscortPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsFixedPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsFlightPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsIntroPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsMissionPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsModelPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsParkingPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsRadioPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsRespawnPage.h"
#include "Windows/Wizards/Awacs/Pages/AwacsTacanPage.h"

class AwacsWizard final : public QWizard
{
public:
	explicit AwacsWizard(const Mission& mission, QWidget* parent = nullptr);

	enum
	{
		IntroPage, RespawnPage, ModelPage, DeparturePage,
		ParkingPage, CarrierPage, FlightPage, MissionPage,
		FixedPage, EscortPage, ChooseEscortPage, RadioPage,
		TacanPage
	};

	[[nodiscard]] Awacs GetAwacs() const;
	void SetAwacs(const Awacs& awacs);

	[[nodiscard]] bool IsDone() const { return m_done; }
	[[nodiscard]] Coalition::Side Coalition() const;
	[[nodiscard]] Awacs InternalGetAwacs() { return m_awacs; }

private slots:
	void configureAwacsModel() const;
	void configureAwacsDeparture() const;
	void configureAwacsEscort() const;
	void finished();

private:
	Awacs m_awacs;
	size_t m_current_page = 0;
	bool m_done = false, m_modified = false;

	AwacsIntroPage* m_intro_page;
	AwacsRespawnPage* m_respawn_page;
	AwacsModelPage* m_model_page;
	AwacsDeparturePage* m_departure_page;
	AwacsParkingPage* m_parking_page;
	AwacsCarrierPage* m_carrier_page;
	AwacsFlightPage* m_flight_page;
	AwacsMissionPage* m_mission_page;
	AwacsFixedPage* m_fixed_page;
	AwacsEscortPage* m_escort_page;
	AwacsChooseEscortPage* m_choose_escort_page;
	AwacsRadioPage* m_radio_page;
	AwacsTacanPage* m_tacan_page;
};
