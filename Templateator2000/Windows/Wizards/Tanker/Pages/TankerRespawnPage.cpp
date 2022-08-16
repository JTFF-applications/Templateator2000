#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerRespawnPage.h"

TankerRespawnPage::TankerRespawnPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Tanker Respawn Options");

	m_label = new QLabel("Do you want the tanker to respawn every time he is killed:", this);
	WRAPPED_HTML_LABEL(m_label)

	m_yes_btn = new QRadioButton("Yes, make it respawn.", this);
	m_no_btn = new QRadioButton("No, he stay dead.", this);

	m_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_error_message)

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_yes_btn);
	m_layout->addWidget(m_no_btn);
	m_layout->addWidget(m_error_message);
	setLayout(m_layout);
}

void TankerRespawnPage::initializePage()
{
	connect(m_yes_btn, &QRadioButton::clicked, [&] { m_error_message->clear(); });
	connect(m_no_btn, &QRadioButton::clicked, [&] { m_error_message->clear(); });
	QWizardPage::initializePage();
}

bool TankerRespawnPage::validatePage()
{
	if (m_yes_btn->isChecked() || m_no_btn->isChecked())
		return true;
	m_error_message->setText("You need to select if you want the tanker to respawn !");
	return false;
}
