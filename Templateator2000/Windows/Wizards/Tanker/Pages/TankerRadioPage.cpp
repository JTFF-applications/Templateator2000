#include "Utilities/Moose.h"
#include "Utilities/Validators/QRealIntValidator.h"
#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerRadioPage.h"

TankerRadioPage::TankerRadioPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Tanker Radio Information");

	m_mhz_label = new QLabel("Mhz", this);

	m_frequency_label = new QLabel("Select the tanker radio frequency (Mhz):", this);
	WRAPPED_HTML_LABEL(m_frequency_label)

	m_frequency = new QLineEdit(this);
	m_frequency->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$"), m_frequency));

	m_frequency_error = new QLabel(this);
	WRAPPED_HTML_LABEL(m_frequency_error)
	LABEL_ERROR_MSG(m_frequency_error)

	m_callsign_label = new QLabel(
		R"(Select the tanker callsign and number <a href="https://google.com">(more info)</a>:)",
		this);
	WRAPPED_HTML_LABEL(m_callsign_label)

	m_callsign_list = new QComboBox(this);
	m_callsign_list->addItems(Moose::GetQtTankerCallsigns());

	m_callsign_number = new QLineEdit(this);
	m_callsign_number->setValidator(new QRealIntValidator(1, 100, m_callsign_number));

	m_callsign_error = new QLabel(this);
	LABEL_ERROR_MSG(m_callsign_error)

	m_layout = new QGridLayout(this);
	m_layout->addWidget(m_frequency_label, 0, 0, 1, 2);
	m_layout->addWidget(m_frequency, 1, 0);
	m_layout->addWidget(m_mhz_label, 1, 1);
	m_layout->addWidget(m_frequency_error, 2, 0, 1, 2);
	m_layout->addWidget(m_callsign_label, 3, 0, 1, 2);
	m_layout->addWidget(m_callsign_list, 4, 0);
	m_layout->addWidget(m_callsign_number, 4, 1);
	m_layout->addWidget(m_callsign_error, 5, 0, 1, 2);
	setLayout(m_layout);
}

void TankerRadioPage::initializePage()
{
	connect(m_frequency, &QLineEdit::textChanged, [&] { m_frequency_error->clear(); });
	connect(m_callsign_number, &QLineEdit::textChanged, [&] { m_callsign_error->clear(); });
	QWizardPage::initializePage();
}

bool TankerRadioPage::validatePage()
{
	QString frequency = m_frequency->text();
	QString callsign_nb = m_callsign_number->text();
	bool res = true;
	int pos = 0;

	if (m_frequency->validator()->validate(frequency, pos) != QValidator::Acceptable)
	{
		m_frequency_error->setText(R"(Invalid frequency ! More info <a href="http://google.com">here</a>)");
		res = false;
	}
	if (m_callsign_number->validator()->validate(callsign_nb, pos) != QValidator::Acceptable)
	{
		m_callsign_error->setText("Invalid callsign number !");
		res = false;
	}
	return res;
}
