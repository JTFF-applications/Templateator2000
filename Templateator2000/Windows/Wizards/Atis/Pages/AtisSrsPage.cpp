#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Atis/Pages/AtisSrsPage.h"

AtisSrsPage::AtisSrsPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Atis - SRS Path");

	m_label = new QLabel("Set the SRS server path. It will use the SR-ExternalAudio.exe "
	                     "to broadcast atis to selected frequency. The default value for JTFF servers is C:\\SRS.",
	                     this);
	WRAPPED_HTML_LABEL(m_label)

	m_srs_path = new QLineEdit(R"(C:\SRS)", this);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_srs_path);
	setLayout(m_layout);
}

bool AtisSrsPage::validatePage()
{
	emit finished();
	return true;
}
