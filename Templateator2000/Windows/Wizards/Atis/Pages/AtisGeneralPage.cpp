#include "Utilities/Validators/QRealIntValidator.h"
#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Atis/Pages/AtisGeneralPage.h"

AtisGeneralPage::AtisGeneralPage(const Mission& mission, QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Atis - General Information");

	m_relay_unit_label = new QLabel(R"(Select the atis <a href="https://google.com">relay unit</a>.)",
	                                this);
	WRAPPED_HTML_LABEL(m_relay_unit_label)

	m_relay_unit = new QComboBox(this);
	QStringList relays;
	std::ranges::transform(mission.GetMission().BeaconUnits(),
	                       std::back_inserter(relays),
	                       [&](const std::string& str) { return QString::fromStdString(str); });
	relays.sort(Qt::CaseInsensitive);
	m_relay_unit->addItems(relays);

	m_relay_unit_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_relay_unit_error_message)

	m_modulation_label = new QLabel(R"(Set the <a href="https://google.com">radio modulation:)", this);
	WRAPPED_HTML_LABEL(m_modulation_label)

	m_modulation = new QComboBox(this);
	m_modulation->addItems({"AM", "FM"});

	m_power_label = new QLabel(R"(Enter <a href="https://google.com">radio power</a> (W):)", this);
	WRAPPED_HTML_LABEL(m_power_label)

	m_power = new QLineEdit(this);
	m_power->setValidator(new QRealIntValidator(0, 100, m_power));

	m_power_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_power_error_message)

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_relay_unit_label);
	m_layout->addWidget(m_relay_unit);
	m_layout->addWidget(m_relay_unit_error_message);
	m_layout->addWidget(m_modulation_label);
	m_layout->addWidget(m_modulation);
	m_layout->addWidget(m_power_label);
	m_layout->addWidget(m_power);
	m_layout->addWidget(m_power_error_message);
	setLayout(m_layout);
}

void AtisGeneralPage::initializePage()
{
	connect(m_power, &QLineEdit::textChanged, [&] { m_power_error_message->clear(); });
	connect(m_relay_unit, &QComboBox::currentTextChanged, [&] { m_relay_unit_error_message->clear(); });
	QWizardPage::initializePage();
}

bool AtisGeneralPage::validatePage()
{
	QString power = m_power->text();
	int pos = 0;
	bool res = true;

	if (m_relay_unit->currentText().isEmpty())
	{
		m_relay_unit_error_message->setText("Please select a relay unit !");
		res = false;
	}
	if (m_power->validator()->validate(power, pos) != QValidator::Acceptable)
	{
		m_power_error_message->setText("Invalid power ! (Range: 0-100)");
		res = false;
	}
	return res;
}
