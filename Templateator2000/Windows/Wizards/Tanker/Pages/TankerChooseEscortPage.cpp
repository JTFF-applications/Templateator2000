#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerChooseEscortPage.h"

TankerChooseEscortPage::TankerChooseEscortPage(const Mission& mission, QWidget* parent)
	: QWizardPage(parent), m_mission(mission)
{
	setTitle("Tanker Escort Selection");

	m_parkings_label = new QLabel("Select the unit who will escort the tanker.", this);
	WRAPPED_HTML_LABEL(m_parkings_label)

	m_escort_list = new QComboBox(this);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_parkings_label);
	m_layout->addWidget(m_escort_list);
	setLayout(m_layout);
}

void TankerChooseEscortPage::initializePage()
{
	QStringList escorts_units;
	std::ranges::transform(m_mission.GetMission().Escorts(reinterpret_cast<TankerWizard*>(wizard())->Coalition()),
	                       std::back_inserter(escorts_units),
	                       [](const std::string& name) { return QString::fromStdString(name); });
	m_escort_list->clear();
	m_escort_list->addItems(escorts_units);

	emit init();

	QWizardPage::initializePage();
}
