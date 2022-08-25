#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>

class AtisSettingsPage final : QWizardPage
{
	Q_OBJECT
	friend class AtisWizard;

public:
	explicit AtisSettingsPage(QWidget* parent = nullptr);

	[[nodiscard]] virtual int nextId() const override;

private:
	QLabel* m_label;
	QCheckBox* m_runway;
	QCheckBox* m_radio;
	QCheckBox* m_tacan;
	QCheckBox* m_ils;
	QVBoxLayout* m_layout;
};
