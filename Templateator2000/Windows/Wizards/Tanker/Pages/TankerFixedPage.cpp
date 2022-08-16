#include <QIntValidator>

#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerFixedPage.h"

TankerFixedPage::TankerFixedPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Tanker Mission Information 2/3");

	m_nm_label_1 = new QLabel("nm", this);
	m_nm_label_2 = new QLabel("nm", this);

	m_racetrack_front_label = new QLabel(
		R"(Select the <a href="https://google.com">racetrack</a> front distance (nm):)",
		this);
	WRAPPED_HTML_LABEL(m_racetrack_front_label)

	m_racetrack_front = new QLineEdit(this);
	m_racetrack_front->setValidator(new QIntValidator(0, 1000, m_racetrack_front));

	m_racetrack_front_error = new QLabel(this);
	LABEL_ERROR_MSG(m_racetrack_front_error)

	m_racetrack_back_label = new QLabel(
		R"(Select the <a href="https://google.com">racetrack</a> front distance (nm):)",
		this);
	WRAPPED_HTML_LABEL(m_racetrack_back_label)

	m_racetrack_back = new QLineEdit(this);
	m_racetrack_back->setValidator(new QIntValidator(0, 1000, m_racetrack_back));

	m_racetrack_back_error = new QLabel(this);
	LABEL_ERROR_MSG(m_racetrack_back_error)

	m_layout = new QGridLayout(this);
	m_layout->addWidget(m_racetrack_front_label, 0, 0);
	m_layout->addWidget(m_racetrack_front, 1, 0);
	m_layout->addWidget(m_nm_label_1, 1, 1);
	m_layout->addWidget(m_racetrack_front_error, 2, 0, 1, 2);
	m_layout->addWidget(m_racetrack_back_label, 3, 0);
	m_layout->addWidget(m_racetrack_back, 4, 0);
	m_layout->addWidget(m_nm_label_2, 4, 1);
	m_layout->addWidget(m_racetrack_back_error, 5, 0, 1, 2);
	setLayout(m_layout);
}

void TankerFixedPage::initializePage()
{
	connect(m_racetrack_front, &QLineEdit::textChanged, [&] { m_racetrack_front_error->clear(); });
	connect(m_racetrack_back, &QLineEdit::textChanged, [&] { m_racetrack_back_error->clear(); });
	QWizardPage::initializePage();
}

bool TankerFixedPage::validatePage()
{
	QString racetrack_front = m_racetrack_front->text();
	QString racetrack_back = m_racetrack_back->text();
	bool res = true;
	int pos = 0;

	if (m_racetrack_front->validator()->validate(racetrack_front, pos) != QValidator::Acceptable)
	{
		m_racetrack_front_error->setText("Invalid racetrack front distance !");
		res = false;
	}
	if (m_racetrack_back->validator()->validate(racetrack_back, pos) != QValidator::Acceptable)
	{
		m_racetrack_back_error->setText("Invalid racetrack back duration !");
		res = false;
	}
	return res;
}

int TankerFixedPage::nextId() const
{
	return TankerWizard::EscortPage;
}
