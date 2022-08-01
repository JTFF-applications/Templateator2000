#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWizardPage>

class TankerMissionPage final : public QWizardPage
{
	Q_OBJECT
	friend class TankerWizard;

public:
	explicit TankerMissionPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_minutes_label;
	QLabel* m_percent_label;
	QLabel* m_nm_label_1;
	QLabel* m_nm_label_2;
	QLabel* m_duration_label;
	QLineEdit* m_duration;
	QLabel* m_duration_error;
	QLabel* m_fuel_level_label;
	QLineEdit* m_fuel_level;
	QLabel* m_fuel_level_error;
	QLabel* m_racetrack_front_label;
	QLineEdit* m_racetrack_front;
	QLabel* m_racetrack_front_error;
	QLabel* m_racetrack_back_label;
	QLineEdit* m_racetrack_back;
	QLabel* m_racetrack_back_error;
	QGridLayout* m_layout;
};
