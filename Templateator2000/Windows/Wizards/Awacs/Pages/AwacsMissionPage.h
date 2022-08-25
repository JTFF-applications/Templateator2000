#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWizardPage>

class AwacsMissionPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsMissionPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_minutes_label;
	QLabel* m_percent_label;
	QLabel* m_duration_label;
	QLineEdit* m_duration;
	QLabel* m_duration_error;
	QLabel* m_fuel_level_label;
	QLineEdit* m_fuel_level;
	QLabel* m_fuel_level_error;
	QGridLayout* m_layout;
};
