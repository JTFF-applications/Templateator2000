#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Beacon/BeaconWizard.h"
#include "Windows/Wizards/Beacon/Pages/BeaconTacanPage.h"

BeaconTacanPage::BeaconTacanPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Tanker Tacan Information");

	m_label = new QLabel("Select the tanker tacan information. "
	                     R"(More info <a href="https://google.com">here</a>.)",
	                     this);
	WRAPPED_HTML_LABEL(m_label)

	m_channel_label = new QLabel("Channel:", this);
	WRAPPED_HTML_LABEL(m_channel_label)

	m_channel = new QLineEdit(this);
	m_channel->setValidator(new QIntValidator(1, 126, m_channel));

	m_channel_error = new QLabel(this);
	LABEL_ERROR_MSG(m_channel_error)

	m_band_label = new QLabel("Band:", this);
	WRAPPED_HTML_LABEL(m_band_label)

	m_band = new QComboBox(this);
	m_band->addItems({"X", "Y"});

	m_morse_label = new QLabel("Morse:", this);
	WRAPPED_HTML_LABEL(m_morse_label)

	m_morse = new QLineEdit(this);
	m_morse->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Z]{3}$"), m_morse));

	m_morse_error = new QLabel(this);
	LABEL_ERROR_MSG(m_morse_error)

	m_layout = new QGridLayout(this);
	m_layout->addWidget(m_label, 0, 0, 1, 2);
	m_layout->addWidget(m_channel_label, 1, 0);
	m_layout->addWidget(m_channel, 1, 1);
	m_layout->addWidget(m_channel_error, 2, 0, 1, 2);
	m_layout->addWidget(m_band_label, 3, 0);
	m_layout->addWidget(m_band, 3, 1);
	m_layout->addWidget(m_morse_label, 4, 0);
	m_layout->addWidget(m_morse, 4, 1);
	m_layout->addWidget(m_morse_error, 5, 0, 1, 2);
	setLayout(m_layout);
}

void BeaconTacanPage::initializePage()
{
	connect(m_channel, &QLineEdit::textChanged, [&] { m_channel_error->clear(); });
	connect(m_morse, &QLineEdit::textChanged, [&] { m_morse_error->clear(); });
	QWizardPage::initializePage();
}

bool BeaconTacanPage::validatePage()
{
	QString channel = m_channel->text();
	QString morse = m_morse->text();
	bool res = true;
	int pos = 0;

	if (m_channel->validator()->validate(channel, pos) != QValidator::Acceptable)
	{
		m_channel_error->setText("Invalid channel !");
		res = false;
	}
	if (m_morse->validator()->validate(morse, pos) != QValidator::Acceptable)
	{
		m_morse_error->setText("Invalid morse code (3 capital letters only) !");
		res = false;
	}
	if (res)
		emit finished();
	return res;
}
