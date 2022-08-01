#pragma once

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWizardPage>

class TankerTacanPage final : public QWizardPage
{
	Q_OBJECT
	friend class TankerWizard;

public:
	explicit TankerTacanPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;

signals:
	// ReSharper disable once CppInconsistentNaming
	void finished();

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
