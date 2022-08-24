#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Atis/AtisWizard.h"
#include "Windows/Wizards/Atis/Pages/AtisIlsPage.h"

AtisIlsPage::AtisIlsPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Atis - ILS Information");

	m_label = new QLabel("Here, you can add an ILS beacon the atis will give.", this);
	WRAPPED_HTML_LABEL(m_label)

	m_runway_label = new QLabel("ILS Runway: ", this);
	WRAPPED_HTML_LABEL(m_runway_label)

	m_runway = new QLineEdit(this);
	m_runway->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[0][1-9]|[1-2][0-9]|[3][0-6][L|R|C]?$"), this));

	m_runway_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_runway_error_message)

	m_frequency_label = new QLabel("ILS Frequency:", this);
	WRAPPED_HTML_LABEL(m_frequency_label)

	m_frequency = new QLineEdit(this);
	m_frequency->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$"), this));

	m_frequency_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_frequency_error_message)

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_runway_label);
	m_layout->addWidget(m_runway);
	m_layout->addWidget(m_runway_error_message);
	m_layout->addWidget(m_frequency_label);
	m_layout->addWidget(m_frequency);
	m_layout->addWidget(m_frequency_error_message);
	setLayout(m_layout);
}

void AtisIlsPage::initializePage()
{
	connect(m_frequency, &QLineEdit::textChanged, [&] { m_frequency_error_message->clear(); });
	connect(m_runway, &QLineEdit::textChanged, [&] { m_runway_error_message->clear(); });
	QWizardPage::initializePage();
}

bool AtisIlsPage::validatePage()
{
	QString frequency = m_frequency->text();
	QString runway = m_runway->text();
	int pos = 0;
	bool res = true;

	if (m_frequency->validator()->validate(frequency, pos) != QValidator::Acceptable)
	{
		m_frequency_error_message->setText("Invalid frequency !");
		res = false;
	}
	if (m_runway->validator()->validate(runway, pos) != QValidator::Acceptable)
	{
		m_runway_error_message->setText("Invalid runway number. Example: 06, 29, 12L, 24R, 36C...");
		res = false;
	}
	return res;
}

int AtisIlsPage::nextId() const
{
	return reinterpret_cast<AtisWizard*>(wizard())->NextOption(AtisWizard::IlsPage);
}
