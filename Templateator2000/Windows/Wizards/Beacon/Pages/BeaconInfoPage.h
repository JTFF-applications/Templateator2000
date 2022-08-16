#pragma once

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWizardPage>

#include "Utilities/Mission.h"

class BeaconInfoPage final : public QWizardPage
{
	Q_OBJECT
	friend class BeaconWizard;

public:
	explicit BeaconInfoPage(const Mission& mission, QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_name_label;
	QLineEdit* m_name;
	QLabel* m_name_error_message;
	QLabel* m_unit_label;
	QComboBox* m_unit;
	QVBoxLayout* m_layout;
};
