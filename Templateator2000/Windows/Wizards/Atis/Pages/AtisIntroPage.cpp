#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Atis/Pages/AtisIntroPage.h"

AtisIntroPage::AtisIntroPage(const Mission& mission, QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Atis - Introduction");

	m_label = new QLabel("This wizard will help you adding an atis to a specific airport.", this);
	WRAPPED_HTML_LABEL(m_label)

	m_airport_label = new QLabel("Select the airport:", this);
	WRAPPED_HTML_LABEL(m_airport_label)

	m_airport = new QComboBox(this);
	QStringList airports;
	std::ranges::transform(Moose::GetAirbasesByMap(mission.GetMission().GetMap()),
	                       std::back_inserter(airports),
	                       [](const std::string& str) { return QString::fromStdString(str); });
	airports.sort(Qt::CaseInsensitive);
	m_airport->addItems(airports);

	m_frequency_label = new QLabel("Select the frequency for the atis:", this);
	WRAPPED_HTML_LABEL(m_frequency_label)

	m_frequency = new QLineEdit(this);
	m_frequency->setValidator(
		new QRegularExpressionValidator(QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$"), this));

	m_frequency_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_frequency_error_message)

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_airport_label);
	m_layout->addWidget(m_airport);
	m_layout->addWidget(m_frequency_label);
	m_layout->addWidget(m_frequency);
	m_layout->addWidget(m_frequency_error_message);
	setLayout(m_layout);
}

void AtisIntroPage::initializePage()
{
	connect(m_frequency, &QLineEdit::textChanged, [&] { m_frequency_error_message->clear(); });
	QWizardPage::initializePage();
}

bool AtisIntroPage::validatePage()
{
	QString frequency = m_frequency->text();
	int pos = 0;

	if (m_frequency->validator()->validate(frequency, pos) != QValidator::Acceptable)
	{
		m_frequency_error_message->setText("Invalid Frequency !");
		return false;
	}
	return true;
}
