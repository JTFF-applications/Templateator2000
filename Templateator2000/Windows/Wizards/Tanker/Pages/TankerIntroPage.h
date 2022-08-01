#pragma once

#include <QComboBox>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWizardPage>

class TankerIntroPage final : public QWizardPage
{
	Q_OBJECT
	friend class TankerWizard;

public:
	explicit TankerIntroPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_label;
	QRadioButton* m_fixed_tanker_btn;
	QRadioButton* m_on_demand_tanker_btn;
	QLabel* m_error_message;
	QLabel* m_coalition_label;
	QComboBox* m_coalition;
	QVBoxLayout* m_layout;
};
