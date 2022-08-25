#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QWizardPage>

class AwacsEscortPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsEscortPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;
	[[nodiscard]] virtual int nextId() const override;

private:
	QLabel* m_escort_label;
	QRadioButton* m_escort_yes;
	QRadioButton* m_escort_no;
	QLabel* m_error_message;
	QVBoxLayout* m_layout;
};
