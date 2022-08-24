#pragma once

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWizardPage>

class AtisTacanPage final : public QWizardPage
{
	Q_OBJECT
	friend class AtisWizard;

public:
	explicit AtisTacanPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;
	virtual int nextId() const override;

private:
	QLabel* m_label;
	QLabel* m_channel_label;
	QLineEdit* m_channel;
	QLabel* m_channel_error;
	QLabel* m_band_label;
	QComboBox* m_band;
	QLabel* m_morse_label;
	QLineEdit* m_morse;
	QLabel* m_morse_error;
	QGridLayout* m_layout;
};
