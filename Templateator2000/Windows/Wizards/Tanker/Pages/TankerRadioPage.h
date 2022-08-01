#pragma once

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWizardPage>

class TankerRadioPage final : public QWizardPage
{
	Q_OBJECT
	friend class TankerWizard;

public:
	explicit TankerRadioPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

private:
	QLabel* m_mhz_label;
	QLabel* m_frequency_label;
	QLineEdit* m_frequency;
	QLabel* m_frequency_error;
	QLabel* m_callsign_label;
	QComboBox* m_callsign_list;
	QLineEdit* m_callsign_number;
	QLabel* m_callsign_error;
	QGridLayout* m_layout;
};
