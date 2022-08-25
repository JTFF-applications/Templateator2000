#pragma once

#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWizardPage>

class AwacsRespawnPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsRespawnPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_label;
	QRadioButton* m_yes_btn;
	QRadioButton* m_no_btn;
	QLabel* m_error_message;
	QVBoxLayout* m_layout;
};
