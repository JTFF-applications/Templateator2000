#include <QIntValidator>

#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerOnDemandPage.h"

TankerOnDemandPage::TankerOnDemandPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Tanker Mission Information 2/3");

	m_degrees_label = new QLabel("°", this);
	m_nm_label = new QLabel("nm", this);

	m_orbit_heading_label = new QLabel(
		R"(Select the <a href="https://google.com">orbit</a> heading (°):)",
		this);
	WRAPPED_HTML_LABEL(m_orbit_heading_label)

	m_orbit_heading = new QLineEdit(this);
	m_orbit_heading->setValidator(new QIntValidator(0, 360, m_orbit_heading));

	m_orbit_heading_error = new QLabel(this);
	LABEL_ERROR_MSG(m_orbit_heading_error)

	m_orbit_length_label = new QLabel(
		R"(Select the <a href="https://google.com">orbit</a> length (nm):)",
		this);
	WRAPPED_HTML_LABEL(m_orbit_length_label)

	m_orbit_length = new QLineEdit(this);
	m_orbit_length->setValidator(new QIntValidator(0, 1000, m_orbit_length));

	m_orbit_length_error = new QLabel(this);
	LABEL_ERROR_MSG(m_orbit_length_error)

	m_layout = new QGridLayout(this);
	m_layout->addWidget(m_orbit_heading_label, 0, 0);
	m_layout->addWidget(m_orbit_heading, 1, 0);
	m_layout->addWidget(m_degrees_label, 1, 1);
	m_layout->addWidget(m_orbit_heading_error, 2, 0, 1, 2);
	m_layout->addWidget(m_orbit_length_label, 3, 0);
	m_layout->addWidget(m_orbit_length, 4, 0);
	m_layout->addWidget(m_nm_label, 4, 1);
	m_layout->addWidget(m_orbit_length_error, 5, 0, 1, 2);
	setLayout(m_layout);
}

void TankerOnDemandPage::initializePage()
{
	connect(m_orbit_heading, &QLineEdit::textChanged, [&] { m_orbit_heading_error->clear(); });
	connect(m_orbit_length, &QLineEdit::textChanged, [&] { m_orbit_length_error->clear(); });
	QWizardPage::initializePage();
}

bool TankerOnDemandPage::validatePage()
{
	QString orbit_heading = m_orbit_heading->text();
	QString orbit_distance = m_orbit_length->text();
	bool res = true;
	int pos = 0;

	if (m_orbit_heading->validator()->validate(orbit_heading, pos) != QValidator::Acceptable)
	{
		m_orbit_heading_error->setText("Invalid orbit heading !");
		res = false;
	}
	if (m_orbit_length->validator()->validate(orbit_distance, pos) != QValidator::Acceptable)
	{
		m_orbit_length_error->setText("Invalid orbit distance !");
		res = false;
	}
	return res;
}

int TankerOnDemandPage::nextId() const
{
	return TankerWizard::RadioPage;
}
