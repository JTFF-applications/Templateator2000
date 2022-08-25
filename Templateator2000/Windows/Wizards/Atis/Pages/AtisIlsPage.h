#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWizardPage>

class AtisIlsPage final : public QWizardPage
{
	Q_OBJECT
	friend class AtisWizard;

public:
	explicit AtisIlsPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;
	[[nodisacrd]] virtual int nextId() const override;

private:
	QLabel* m_label;
	QLabel* m_runway_label;
	QLineEdit* m_runway;
	QLabel* m_runway_error_message;
	QLabel* m_frequency_label;
	QLineEdit* m_frequency;
	QLabel* m_frequency_error_message;
	QVBoxLayout* m_layout;
};
