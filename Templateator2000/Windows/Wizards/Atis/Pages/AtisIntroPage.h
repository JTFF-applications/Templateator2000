#pragma once

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWizardPage>

#include "Utilities/Mission.h"

class AtisIntroPage final : public QWizardPage
{
	Q_OBJECT
	friend class AtisWizard;

public:
	explicit AtisIntroPage(const Mission& mission, QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_label;
	QLabel* m_airport_label;
	QComboBox* m_airport;
	QLabel* m_frequency_label;
	QLineEdit* m_frequency;
	QLabel* m_frequency_error_message;
	QVBoxLayout* m_layout;
};
