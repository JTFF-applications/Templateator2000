﻿#pragma once

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>

#include "Utilities/Mission.h"

class TankerModelPage final : public QWizardPage
{
	Q_OBJECT
	friend class TankerWizard;

public:
	explicit TankerModelPage(const Mission& mission, QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

signals:
	// ReSharper disable once CppInconsistentNaming
	void init();

private:
	const Mission& m_mission;

	QLabel* m_group_label;
	QComboBox* m_group_list;
	QLabel* m_group_error_message;
	QLabel* m_pattern_label;
	QComboBox* m_pattern_list;
	QLabel* m_pattern_error_message;
	QVBoxLayout* m_layout;
};
