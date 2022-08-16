#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Beacon/Pages/BeaconIntroPage.h"

BeaconIntroPage::BeaconIntroPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Introduction - Beacon");

	m_label = new QLabel("This wizard will help you to create a new Beacon.", this);
	WRAPPED_HTML_LABEL(m_label)

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_label);
}
