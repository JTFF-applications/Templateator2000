#pragma once

#include "Utilities/Mission.h"
#include "Windows/Wizards/Atis/Pages/AtisGeneralPage.h"
#include "Windows/Wizards/Atis/Pages/AtisIlsPage.h"
#include "Windows/Wizards/Atis/Pages/AtisIntroPage.h"
#include "Windows/Wizards/Atis/Pages/AtisRadioPage.h"
#include "Windows/Wizards/Atis/Pages/AtisRunwayPage.h"
#include "Windows/Wizards/Atis/Pages/AtisSettingsPage.h"
#include "Windows/Wizards/Atis/Pages/AtisSrsPage.h"
#include "Windows/Wizards/Atis/Pages/AtisTacanPage.h"

class AtisWizard final : public QWizard
{
public:
	explicit AtisWizard(const Mission& mission, QWidget* parent = nullptr);

	enum
	{
		IntroPage, GeneralPage, SettingsPage, RunwayPage,
		RadioPage, TacanPage, IlsPage, SrsPage
	};

	[[nodiscard]] Atis GetAtis() const;
	void SetAtis(const Atis& atis);

	[[nodiscard]] bool IsDone() const { return m_done; }
	[[nodiscard]] int NextOption(const int& current_page) const;

private slots:
	void finished();

private:
	Atis m_atis;
	bool m_done = false, m_modified = false;

	AtisIntroPage* m_intro_page;
	AtisGeneralPage* m_general_page;
	AtisSettingsPage* m_settings_page;
	AtisRunwayPage* m_runway_page;
	AtisRadioPage* m_radio_page;
	AtisTacanPage* m_tacan_page;
	AtisIlsPage* m_ils_page;
	AtisSrsPage* m_srs_page;
};
