#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWizardPage>

class TankerCarrierPage final : public QWizardPage
{
	Q_OBJECT
	friend class TankerWizard;

public:
	explicit TankerCarrierPage(QWidget* parent = nullptr);

	virtual int nextId() const override;

private:
	QLabel* m_modex_label;
	QLineEdit* m_modex;
	QLabel* m_airboss_label;
	QRadioButton* m_airboss_yes;
	QRadioButton* m_airboss_no;
	QVBoxLayout* m_layout;
};
