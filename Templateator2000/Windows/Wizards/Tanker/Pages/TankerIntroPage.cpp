#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerIntroPage.h"

TankerIntroPage::TankerIntroPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Introduction - Type of Tanker");
	setSubTitle("This wizard will help you adding a tanker to your mission.");

	m_label = new QLabel("Select the type of tanker you want to add:<br>"
	                     R"(- The <a href="https://google.com">Fixed Tanker</a> which cannot be moved during the mission<br>)"
	                     R"(- The <a href="https://google.com">On Demand Tanker</a> which can be moved during mission with marks on the F10 map<br>)",
	                     this);
	WRAPPED_HTML_LABEL(m_label)

	m_fixed_tanker_btn = new QRadioButton("Add a fixed tanker.", this);
	m_on_demand_tanker_btn = new QRadioButton("Add an on demand tanker.", this);

	m_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_error_message)

	m_coalition_label = new QLabel("Select the tanker coalition:", this);

	m_coalition = new QComboBox(this);
	m_coalition->addItems({"Blue", "Red", "Neutral"});

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_label);
	m_layout->addWidget(m_fixed_tanker_btn);
	m_layout->addWidget(m_on_demand_tanker_btn);
	m_layout->addWidget(m_error_message);
	m_layout->addWidget(m_coalition_label);
	m_layout->addWidget(m_coalition);
	setLayout(m_layout);
}

void TankerIntroPage::initializePage()
{
	connect(m_fixed_tanker_btn, &QRadioButton::clicked, [&] { m_error_message->clear(); });
	connect(m_on_demand_tanker_btn, &QRadioButton::clicked, [&] { m_error_message->clear(); });
	QWizardPage::initializePage();
}

bool TankerIntroPage::validatePage()
{
	if (m_fixed_tanker_btn->isChecked() || m_on_demand_tanker_btn->isChecked())
		return true;
	m_error_message->setText("You need to select a tanker type !");
	return false;
}
