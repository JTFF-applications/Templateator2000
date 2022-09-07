#pragma once

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWizardPage>

#include "Utilities/Mission.h"

class AwacsRadioPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsRadioPage(const Mission& mission, QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	Mission m_mission;

	QLabel* m_mhz_label;
	QLabel* m_frequency_label;
	QLineEdit* m_frequency;
	QLabel* m_frequency_error;
	QLabel* m_callsign_label;
	QComboBox* m_callsign_list;
	QLineEdit* m_callsign_number;
	QLabel* m_callsign_error;
	QGridLayout* m_layout;
};
