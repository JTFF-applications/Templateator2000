#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Atis/AtisWizard.h"
#include "Windows/Wizards/Atis/Pages/AtisSettingsPage.h"

AtisSettingsPage::AtisSettingsPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Atis - General Settings");

	m_label = new QLabel("Select the atis settings. "
	                     "If not selected, the information will not be into the atis, or will be calculated by DCS.<br>"
	                     R"(More info <a href="https://google.com>here</a>)",
	                     this);
	WRAPPED_HTML_LABEL(m_label)

	m_runway = new QCheckBox("Force Active Runway", this);
	m_radio = new QCheckBox("Include Tower Frequencies", this);
	m_tacan = new QCheckBox("Include Tacan", this);
	m_ils = new QCheckBox("Include Ils", this);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_runway);
	m_layout->addWidget(m_radio);
	m_layout->addWidget(m_tacan);
	m_layout->addWidget(m_ils);
	setLayout(m_layout);
}

int AtisSettingsPage::nextId() const
{
	if (m_runway->isChecked())
		return AtisWizard::RunwayPage;
	if (m_radio->isChecked())
		return AtisWizard::RadioPage;
	if (m_tacan->isChecked())
		return AtisWizard::TacanPage;
	if (m_ils->isChecked())
		return AtisWizard::IlsPage;
	return AtisWizard::SrsPage;
}
