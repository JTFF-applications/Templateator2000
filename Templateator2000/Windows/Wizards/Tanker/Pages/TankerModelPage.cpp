#include "Windows/Wizards/WizardMacros.h"
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

	m_group_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_group_error_message)

	m_pattern_label = new QLabel(
		R"(Select the orbit point for the tanker <a href="http://google.com">(more info)</a>:)",
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

void TankerModelPage::initializePage()
{
	connect(m_group_list, &QComboBox::currentTextChanged, [&] { m_group_error_message->clear(); });
	connect(m_pattern_list, &QComboBox::currentTextChanged, [&] { m_pattern_error_message->clear(); });

	QStringList tanker_groups;
	std::ranges::transform(m_mission.GetMission().Tankers(reinterpret_cast<TankerWizard*>(wizard())->Coalition()),
	                       std::back_inserter(tanker_groups),
	                       [](const std::string& type) { return QString::fromStdString(type); });
	tanker_groups.sort(Qt::CaseInsensitive);
	m_group_list->clear();
	m_group_list->addItems(tanker_groups);

	QStringList pattern_units;
	std::ranges::transform(
		m_mission.GetMission().OrbitUnits(reinterpret_cast<TankerWizard*>(wizard())->Coalition()),
		std::back_inserter(pattern_units),
		[](const std::string& type) { return QString::fromStdString(type); });
	pattern_units.sort(Qt::CaseInsensitive);
	m_pattern_list->clear();
	m_pattern_list->addItems(pattern_units);

	if (reinterpret_cast<TankerWizard*>(wizard())->Type() == Tanker::Type::OnDemand)
	{
		m_pattern_label->setVisible(false);
		m_pattern_list->setVisible(false);
	}

	emit init();

	QWizardPage::initializePage();
}

bool TankerModelPage::validatePage()
{
	bool is_valid = true;

	if (m_group_list->currentText().isEmpty())
	{
		m_group_error_message->setText("You need to select a model group for the tanker !");
		is_valid = false;
	}
	if (m_pattern_list->currentText().isEmpty() && m_pattern_list->isVisible())
	{
		m_pattern_error_message->setText("You need to select a pattern unit for the tanker !");
		is_valid = false;
	}

	return is_valid;
}
