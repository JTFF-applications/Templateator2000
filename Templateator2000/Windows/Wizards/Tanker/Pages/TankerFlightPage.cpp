#include <QIntValidator>

#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerFlightPage.h"

TankerFlightPage::TankerFlightPage(QWidget* parent)
{
	setTitle("Tanker Flight Information");

	m_feet_label = new QLabel("ft", this);
	m_knots_label = new QLabel("kts", this);

	m_altitude_label = new QLabel("Select the tanker altitude (ft):", this);
	WRAPPED_HTML_LABEL(m_altitude_label)

	m_altitude = new QLineEdit(this);
	m_altitude->setValidator(new QIntValidator(0, 100000, m_altitude));

	m_altitude_error = new QLabel(this);
	LABEL_ERROR_MSG(m_altitude_error)

	m_speed_label = new QLabel("Select the tanker ground speed (kts):", this);
	WRAPPED_HTML_LABEL(m_speed_label)

	m_speed = new QLineEdit(this);
	m_speed->setValidator(new QIntValidator(0, 1000, m_speed));

	m_speed_error = new QLabel(this);
	LABEL_ERROR_MSG(m_speed_error)

	m_layout = new QGridLayout(this);
	m_layout->addWidget(m_altitude_label, 0, 0, 1, 2);
	m_layout->addWidget(m_altitude, 1, 0);
	m_layout->addWidget(m_feet_label, 1, 1);
	m_layout->addWidget(m_altitude_error, 2, 0, 1, 2);
	m_layout->addWidget(m_speed_label, 3, 0, 1, 2);
	m_layout->addWidget(m_speed, 4, 0);
	m_layout->addWidget(m_knots_label, 4, 1);
	m_layout->addWidget(m_speed_error, 5, 0, 1, 2);
	setLayout(m_layout);
}

void TankerFlightPage::initializePage()
{
	connect(m_altitude, &QLineEdit::textChanged, [&] { m_altitude_error->clear(); });
	connect(m_speed, &QLineEdit::textChanged, [&] { m_speed_error->clear(); });
	QWizardPage::initializePage();
}

bool TankerFlightPage::validatePage()
{
	QString altitude = m_altitude->text();
	QString speed = m_speed->text();
	bool res = true;
	int pos = 0;

	if (m_altitude->validator()->validate(altitude, pos) != QValidator::Acceptable)
	{
		res = false;
		m_altitude_error->setText("Invalid altitude !");
	}
	if (m_speed->validator()->validate(speed, pos) != QValidator::Acceptable)
	{
		res = false;
		m_speed_error->setText("Invalid speed !");
	}
	return res;
}
