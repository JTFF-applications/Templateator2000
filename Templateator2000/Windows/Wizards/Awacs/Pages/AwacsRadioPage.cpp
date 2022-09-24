#include "Utilities/Moose.h"
#include "Utilities/Validators/QRealIntValidator.h"
#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Awacs/AwacsWizard.h"
#include "Windows/Wizards/Awacs/Pages/AwacsRadioPage.h"

AwacsRadioPage::AwacsRadioPage(const Mission& mission, QWidget* parent)
	: QWizardPage(parent), m_mission(mission)
{
	setTitle("Awacs Radio Information");

	m_mhz_label = new QLabel("Mhz", this);

	m_frequency_label = new QLabel("Select the awacs radio frequency (Mhz):", this);
	WRAPPED_HTML_LABEL(m_frequency_label)

	m_frequency = new QLineEdit(this);
	m_frequency->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$"), m_frequency));

	m_frequency_error = new QLabel(this);
	WRAPPED_HTML_LABEL(m_frequency_error)
	LABEL_ERROR_MSG(m_frequency_error)

	m_callsign_label = new QLabel(
		R"(Select the awacs callsign and number <a href="https://google.com">(more info)</a>:)",
		this);
	WRAPPED_HTML_LABEL(m_callsign_label)

	m_callsign_list = new QComboBox(this);
	m_callsign_list->addItems(Moose::GetQtAwacsCallsigns());

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

void AwacsRadioPage::initializePage()
{
	connect(m_frequency, &QLineEdit::textChanged, [&] { m_frequency_error->clear(); });
	connect(m_callsign_list, &QComboBox::currentTextChanged, [&] { m_callsign_error->clear(); });
	connect(m_callsign_number, &QLineEdit::textChanged, [&] { m_callsign_error->clear(); });
	QWizardPage::initializePage();
}

bool AwacsRadioPage::validatePage()
{
	QString frequency = m_frequency->text();
	QString callsign_nb = m_callsign_number->text();
	bool res = true;
	int pos = 0;

	const auto& awacs_list = m_mission.GetScripts().GetAwacs();
	const auto& modified_awacs = reinterpret_cast<AwacsWizard*>(wizard())->InternalGetAwacs();
	bool has_unique_callsign = true;
	for (const auto& awacs : awacs_list)
		if (awacs.Callsign.Name == m_callsign_list->currentText().toStdString() && awacs.Callsign.Number ==
		    callsign_nb.toInt() && awacs != modified_awacs)
			has_unique_callsign = false;

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
	else if (!has_unique_callsign)
	{
		m_callsign_error->setText("Callsign is already in use by another awacs !");
		res = false;
	}
	return res;
}
