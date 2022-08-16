#pragma once

#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>

class BeaconIntroPage final : public QWizardPage
{
	Q_OBJECT
	friend class BeaconWizard;

public:
	explicit BeaconIntroPage(QWidget* parent = nullptr);

private:
	QLabel* m_label;
	QVBoxLayout* m_layout;
};
