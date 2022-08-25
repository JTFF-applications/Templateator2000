﻿#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWizardPage>

class AwacsFixedPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsFixedPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_nm_label_1;
	QLabel* m_nm_label_2;
	QLabel* m_racetrack_front_label;
	QLineEdit* m_racetrack_front;
	QLabel* m_racetrack_front_error;
	QLabel* m_racetrack_back_label;
	QLineEdit* m_racetrack_back;
	QLabel* m_racetrack_back_error;
	QGridLayout* m_layout;
};
