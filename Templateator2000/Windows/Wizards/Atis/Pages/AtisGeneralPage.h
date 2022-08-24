#pragma once

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWizardPage>

#include "Utilities/Mission.h"

class AtisGeneralPage final : public QWizardPage
{
	Q_OBJECT
	friend class AtisWizard;

public:
	explicit AtisGeneralPage(const Mission& mission, QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_relay_unit_label;
	QComboBox* m_relay_unit;
	QLabel* m_relay_unit_error_message;
	QLabel* m_modulation_label;
	QComboBox* m_modulation;
	QLabel* m_power_label;
	QLineEdit* m_power;
	QLabel* m_power_error_message;
	QVBoxLayout* m_layout;
};
