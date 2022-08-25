#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Awacs/AwacsWizard.h"
#include "Windows/Wizards/Awacs/Pages/AwacsModelPage.h"

AwacsModelPage::AwacsModelPage(const Mission& mission, QWidget* parent)
	: QWizardPage(parent), m_mission(mission)
{
	setTitle("Awacs Model");

	m_group_label = new QLabel(
		R"(Select the model group for the awacs <a href="http://google.com">(more info)</a>:)",
		this);
	WRAPPED_HTML_LABEL(m_group_label)

	m_group_list = new QComboBox(this);

	m_group_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_group_error_message)

	m_pattern_label = new QLabel(
		R"(Select the orbit point for the awacs <a href="http://google.com">(more info)</a>:)",
		this);
	WRAPPED_HTML_LABEL(m_pattern_label)

	m_pattern_list = new QComboBox(this);

	m_pattern_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_pattern_error_message)

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_group_label);
	m_layout->addWidget(m_group_list);
	m_layout->addWidget(m_group_error_message);
	m_layout->addWidget(m_pattern_label);
	m_layout->addWidget(m_pattern_list);
	m_layout->addWidget(m_pattern_error_message);
	setLayout(m_layout);
}

void AwacsModelPage::initializePage()
{
	connect(m_group_list, &QComboBox::currentTextChanged, [&] { m_group_error_message->clear(); });
	connect(m_pattern_list, &QComboBox::currentTextChanged, [&] { m_pattern_error_message->clear(); });

	QStringList tanker_groups;
	std::ranges::transform(m_mission.GetMission().Awacs(reinterpret_cast<AwacsWizard*>(wizard())->Coalition()),
	                       std::back_inserter(tanker_groups),
	                       [](const std::string& type) { return QString::fromStdString(type); });
	tanker_groups.sort(Qt::CaseInsensitive);
	m_group_list->clear();
	m_group_list->addItems(tanker_groups);

	QStringList pattern_units;
	std::ranges::transform(
		m_mission.GetMission().OrbitUnits(reinterpret_cast<AwacsWizard*>(wizard())->Coalition()),
		std::back_inserter(pattern_units),
		[](const std::string& type) { return QString::fromStdString(type); });
	pattern_units.sort(Qt::CaseInsensitive);
	m_pattern_list->clear();
	m_pattern_list->addItems(pattern_units);

	emit init();

	QWizardPage::initializePage();
}

bool AwacsModelPage::validatePage()
{
	bool is_valid = true;

	if (m_group_list->currentText().isEmpty())
	{
		m_group_error_message->setText("You need to select a model group for the awacs !");
		is_valid = false;
	}
	if (m_pattern_list->currentText().isEmpty() && m_pattern_list->isVisible())
	{
		m_pattern_error_message->setText("You need to select a pattern unit for the awacs !");
		is_valid = false;
	}

	return is_valid;
}
