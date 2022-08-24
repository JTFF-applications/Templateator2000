#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Atis/AtisWizard.h"
#include "Windows/Wizards/Atis/Pages/AtisRunwayPage.h"

AtisRunwayPage::AtisRunwayPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Atis - Force Active Runway");

	m_label = new QLabel("Set the active runway:", this);
	WRAPPED_HTML_LABEL(m_label)

	m_runway = new QLineEdit(this);
	m_runway->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[0][1-9]|[1-2][0-9]|[3][0-6][L|R|C]?$"), m_runway));

	m_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_error_message)

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_runway);
	m_layout->addWidget(m_error_message);
	setLayout(m_layout);
}

void AtisRunwayPage::initializePage()
{
	connect(m_runway, &QLineEdit::textChanged, [&] { m_error_message->clear(); });
	QWizardPage::initializePage();
}

bool AtisRunwayPage::validatePage()
{
	QString runway = m_runway->text();
	int pos = 0;

	if (m_runway->validator()->validate(runway, pos) != QValidator::Acceptable)
	{
		m_error_message->setText("Invalid runway number. Example: 06, 29, 12L, 24R, 36C...");
		return false;
	}
	return true;
}

int AtisRunwayPage::nextId() const
{
	return reinterpret_cast<AtisWizard*>(wizard())->NextOption(AtisWizard::RunwayPage);
}
