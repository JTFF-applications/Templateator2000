#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWizardPage>

class TankerOnDemandFlightPage final : public QWizardPage
{
	Q_OBJECT
	friend class TankerWizard;

public:
	explicit TankerOnDemandFlightPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;
	[[nodiscard]] virtual int nextId() const override;

private:
	QLabel* m_name_label;
	QLineEdit* m_name;
	QLabel* m_name_error_message;
	QLabel* m_default_label;
	QRadioButton* m_yes_btn;
	QRadioButton* m_no_btn;
	QLabel* m_default_error_message;
	QVBoxLayout* m_layout;
};
