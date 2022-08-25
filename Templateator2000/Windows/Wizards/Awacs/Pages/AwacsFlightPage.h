#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWizardPage>

class AwacsFlightPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsFlightPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_feet_label;
	QLabel* m_knots_label;
	QLabel* m_altitude_label;
	QLineEdit* m_altitude;
	QLabel* m_altitude_error;
	QLabel* m_speed_label;
	QLineEdit* m_speed;
	QLabel* m_speed_error;
	QGridLayout* m_layout;
};
