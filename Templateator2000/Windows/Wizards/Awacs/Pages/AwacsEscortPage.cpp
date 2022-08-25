#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Awacs/AwacsWizard.h"
#include "Windows/Wizards/Awacs/Pages/AwacsEscortPage.h"

AwacsEscortPage::AwacsEscortPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Awacs Mission Information 3/3");

	m_escort_label = new QLabel("Do you want the awacs to be escorted during its mission?<br>", this);
	WRAPPED_HTML_LABEL(m_escort_label)

	m_escort_yes = new QRadioButton("Yes, escort my awacs.", this);
	m_escort_no = new QRadioButton("No, do not escort my awacs.", this);

	m_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_error_message)

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_escort_label);
	m_layout->addWidget(m_escort_yes);
	m_layout->addWidget(m_escort_no);
	m_layout->addWidget(m_error_message);
	setLayout(m_layout);
}

void AwacsEscortPage::initializePage()
{
	connect(m_escort_yes, &QRadioButton::clicked, [&] { m_error_message->clear(); });
	connect(m_escort_no, &QRadioButton::clicked, [&] { m_error_message->clear(); });
	QWizardPage::initializePage();
}

bool AwacsEscortPage::validatePage()
{
	if (m_escort_yes->isChecked() || m_escort_no->isChecked())
		return true;
	m_error_message->setText("You need to select if you want an escort !");
	return false;
}

int AwacsEscortPage::nextId() const
{
	if (m_escort_no->isChecked())
		return AwacsWizard::RadioPage;
	return AwacsWizard::ChooseEscortPage;
}
