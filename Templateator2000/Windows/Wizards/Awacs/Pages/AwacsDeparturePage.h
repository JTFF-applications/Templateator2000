#pragma once

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>

#include "Utilities/Mission.h"

class AwacsDeparturePage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsDeparturePage(const Mission& mission, QWidget* parent = nullptr);

	virtual void initializePage() override;
	[[nodiscard]] virtual int nextId() const override;

signals:
	// ReSharper disable once CppInconsistentNaming
	void init();

private:
	const Mission& m_mission;

	QLabel* m_departure_label;
	QComboBox* m_departure_list;
	QVBoxLayout* m_layout;
};
