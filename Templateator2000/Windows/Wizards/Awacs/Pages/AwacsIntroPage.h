#pragma once

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>

class AwacsIntroPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsIntroPage(QWidget* parent = nullptr);

private:
	QLabel* m_label;
	QComboBox* m_coalition;
	QVBoxLayout* m_layout;
};
