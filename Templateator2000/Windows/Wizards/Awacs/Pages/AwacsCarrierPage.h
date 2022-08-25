#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWizardPage>

class AwacsCarrierPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsCarrierPage(QWidget* parent = nullptr);

private:
	QLabel* m_modex_label;
	QLineEdit* m_modex;
	QLabel* m_airboss_label;
	QRadioButton* m_airboss_yes;
	QRadioButton* m_airboss_no;
	QVBoxLayout* m_layout;
};
