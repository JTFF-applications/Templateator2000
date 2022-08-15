#pragma once

#include "Models/Tanker.h"
#include "Utilities/Mission.h"
#include "Windows/Wizards/Tanker/Pages/TankerCarrierPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerChooseEscortPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerDeparturePage.h"
#include "Windows/Wizards/Tanker/Pages/TankerEscortPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerFixedPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerFlightPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerIntroPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerMissionPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerModelPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerOnDemandFlightPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerOnDemandPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerParkingPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerRadioPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerRespawnPage.h"
#include "Windows/Wizards/Tanker/Pages/TankerTacanPage.h"

#define WRAPPED_HTML_LABEL(label) (label)->setTextFormat(Qt::RichText);\
								  (label)->setTextInteractionFlags(Qt::TextBrowserInteraction);\
								  (label)->setOpenExternalLinks(true);\
								  (label)->setWordWrap(true);

#define LABEL_ERROR_MSG(label) (label)->setStyleSheet("QLabel { color : red; }");

class TankerWizard final : public QWizard
{
public:
	explicit TankerWizard(const Mission& mission, QWidget* parent = nullptr);

	enum
	{
		IntroPage, RespawnPage, ModelPage, DeparturePage,
		ParkingPage, CarrierPage, OnDemandFlightPage, FlightPage,
		MissionPage, FixedPage, OnDemandPage, EscortPage,
		ChooseEscortPage, RadioPage, TacanPage
	};

	[[nodiscard]] Tanker GetTanker() const;
	void SetTanker(const Tanker& tanker);

	[[nodiscard]] bool IsDone() const { return m_done; }
	[[nodiscard]] bool IsCustomValues() const;
	[[nodiscard]] Coalition::Side Coalition() const;
	[[nodiscard]] enum Tanker::Type Type() const;

private slots:
	void configureTankerModel() const;
	void configureTankerDeparture() const;
	void configureTankerEscort() const;
	void finished();

private:
	Tanker m_tanker;
	size_t m_current_page = 0;
	bool m_done = false, m_modified = false;

	TankerIntroPage* m_intro_page;
	TankerRespawnPage* m_respawn_page;
	TankerModelPage* m_model_page;
	TankerDeparturePage* m_departure_page;
	TankerParkingPage* m_parking_page;
	TankerCarrierPage* m_carrier_page;
	TankerOnDemandFlightPage* m_ondemand_flight_page;
	TankerFlightPage* m_flight_page;
	TankerMissionPage* m_mission_page;
	TankerFixedPage* m_fixed_page;
	TankerOnDemandPage* m_ondemand_page;
	TankerEscortPage* m_escort_page;
	TankerChooseEscortPage* m_choose_escort_page;
	TankerRadioPage* m_radio_page;
	TankerTacanPage* m_tacan_page;
};
