#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Awacs/AwacsWizard.h"
#include "Windows/Wizards/Awacs/Pages/AwacsIntroPage.h"

AwacsIntroPage::AwacsIntroPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Introduction - Awacs Coalition");
	setSubTitle("This wizard will help you adding an awacs to your mission.");

	m_label = new QLabel("Select the awacs coalition:", this);
	WRAPPED_HTML_LABEL(m_label)

	m_coalition = new QComboBox(this);
	m_coalition->addItems({"Blue", "Red", "Neutral"});

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_coalition);
	setLayout(m_layout);
}
