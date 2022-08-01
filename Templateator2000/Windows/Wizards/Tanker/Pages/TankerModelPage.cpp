#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerModelPage.h"

TankerModelPage::TankerModelPage(const Mission& mission, QWidget* parent)
	: QWizardPage(parent), m_mission(mission)
{
	setTitle("Tanker Model");

	m_group_label = new QLabel(
		R"(Select the model group for the tanker <a href="http://google.com">(more info)</a>:)",
		this);
	WRAPPED_HTML_LABEL(m_group_label)

	m_group_list = new QComboBox(this);

	m_pattern_label = new QLabel(
		R"(Select the orbit point for the tanker <a href="http://google.com">(more info)</a>:)",
		this);
	WRAPPED_HTML_LABEL(m_pattern_label)

	m_pattern_list = new QComboBox(this);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_group_label);
	m_layout->addWidget(m_group_list);
	m_layout->addWidget(m_pattern_label);
	m_layout->addWidget(m_pattern_list);
	setLayout(m_layout);
}

void TankerModelPage::initializePage()
{
	QStringList tanker_groups;
	std::ranges::transform(m_mission.GetMission().Tankers(reinterpret_cast<TankerWizard*>(wizard())->Coalition()),
	                       std::back_inserter(tanker_groups),
	                       [](const std::string& type) { return QString::fromStdString(type); });
	tanker_groups.sort(Qt::CaseInsensitive);
	m_group_list->clear();
	m_group_list->addItems(tanker_groups);

	QStringList pattern_units;
	std::ranges::transform(m_mission.GetMission().OrbitUnits(reinterpret_cast<TankerWizard*>(wizard())->Coalition()),
	                       std::back_inserter(pattern_units),
	                       [](const std::string& type) { return QString::fromStdString(type); });
	pattern_units.sort(Qt::CaseInsensitive);
	m_pattern_list->clear();
	m_pattern_list->addItems(pattern_units);

	emit init();

	QWizardPage::initializePage();
}
