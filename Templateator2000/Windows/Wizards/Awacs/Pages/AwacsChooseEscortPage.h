#pragma once

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>

#include "Utilities/Mission.h"

class AwacsChooseEscortPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsChooseEscortPage(const Mission& mission, QWidget* parent = nullptr);

	virtual void initializePage() override;

signals:
	// ReSharper disable once CppInconsistentNaming
	void init();

private:
	const Mission& m_mission;

	QLabel* m_parkings_label;
	QComboBox* m_escort_list;
	QVBoxLayout* m_layout;
};
