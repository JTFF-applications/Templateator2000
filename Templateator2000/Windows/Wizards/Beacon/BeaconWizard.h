#pragma once

#include "Models/Beacon.h"
#include "Utilities/Mission.h"

#include "Windows/Wizards/Beacon/Pages/BeaconInfoPage.h"
#include "Windows/Wizards/Beacon/Pages/BeaconIntroPage.h"
#include "Windows/Wizards/Beacon/Pages/BeaconTacanPage.h"

class BeaconWizard final : public QWizard
{
public:
	explicit BeaconWizard(const Mission& mission, QWidget* parent = nullptr);

	enum
	{
		IntroPage, InfoPage, TacanPage
	};

	[[nodiscard]] Beacon GetBeacon() const;
	void SetBeacon(const Beacon& beacon);

	[[nodiscard]] bool IsDone() const { return m_done; }

private slots:
	void finished();

private:
	Beacon m_beacon;
	bool m_done = false, m_modified = false;

	BeaconIntroPage* m_intro_page;
	BeaconInfoPage* m_info_page;
	BeaconTacanPage* m_tacan_page;
};
