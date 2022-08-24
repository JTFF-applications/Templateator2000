#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWizardPage>

class AtisRunwayPage final : public QWizardPage
{
	Q_OBJECT
	friend class AtisWizard;

public:
	explicit AtisRunwayPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;
	virtual int nextId() const override;

private:
	QLabel* m_label;
	QLineEdit* m_runway;
	QLabel* m_error_message;
	QVBoxLayout* m_layout;
};
