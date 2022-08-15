#include <QIntValidator>

#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerMissionPage.h"

TankerMissionPage::TankerMissionPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Tanker Mission Information 1/3");

	m_minutes_label = new QLabel("min", this);
	m_percent_label = new QLabel("%", this);

	m_duration_label = new QLabel("Select the maximum mission duration "
	                              "for the tanker (min):",
	                              this);
	WRAPPED_HTML_LABEL(m_duration_label)

	m_duration = new QLineEdit(this);
	m_duration->setValidator(new QIntValidator(0, 1440, m_duration));

	m_duration_error = new QLabel(this);
	LABEL_ERROR_MSG(m_duration_error)

	m_fuel_level_label = new QLabel("Select the fuel level for the tanker to RTB (%):", this);
	WRAPPED_HTML_LABEL(m_fuel_level_label)

	m_fuel_level = new QLineEdit(this);
	m_fuel_level->setValidator(new QIntValidator(0, 100, m_fuel_level));

	m_fuel_level_error = new QLabel(this);
	LABEL_ERROR_MSG(m_fuel_level_error)

	m_layout = new QGridLayout(this);
	m_layout->addWidget(m_duration_label, 0, 0, 1, 2);
	m_layout->addWidget(m_duration, 1, 0);
	m_layout->addWidget(m_minutes_label, 1, 1);
	m_layout->addWidget(m_duration_error, 2, 0, 1, 2);
	m_layout->addWidget(m_fuel_level_label, 3, 0, 1, 2);
	m_layout->addWidget(m_fuel_level, 4, 0);
	m_layout->addWidget(m_percent_label, 4, 1);
	m_layout->addWidget(m_fuel_level_error, 5, 0, 1, 2);
	setLayout(m_layout);
}

void TankerMissionPage::initializePage()
{
	connect(m_duration, &QLineEdit::textChanged, [&] { m_duration_error->clear(); });
	connect(m_fuel_level, &QLineEdit::textChanged, [&] { m_fuel_level_error->clear(); });
	QWizardPage::initializePage();
}

bool TankerMissionPage::validatePage()
{
	QString duration = m_duration->text();
	QString fuel_level = m_fuel_level->text();
	bool res = true;
	int pos = 0;

	if (m_duration->validator()->validate(duration, pos) != QValidator::Acceptable)
	{
		m_duration_error->setText("Invalid mission maximum duration !");
		res = false;
	}
	if (m_fuel_level->validator()->validate(fuel_level, pos) != QValidator::Acceptable)
	{
		m_fuel_level_error->setText("Invalid fuel level !");
		res = false;
	}
	return res;
}

int TankerMissionPage::nextId() const
{
	if (reinterpret_cast<TankerWizard*>(wizard())->Type() == Tanker::Type::Fixed)
		return TankerWizard::FixedPage;
	if (reinterpret_cast<TankerWizard*>(wizard())->IsCustomValues())
		return TankerWizard::OnDemandPage;
	return TankerWizard::RadioPage;
}
