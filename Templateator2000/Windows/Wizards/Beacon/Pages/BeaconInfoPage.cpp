#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Beacon/Pages/BeaconInfoPage.h"

BeaconInfoPage::BeaconInfoPage(const Mission& mission, QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Beacon Name and Unit Information");

	m_name_label = new QLabel("Enter the name of the beacon:", this);
	WRAPPED_HTML_LABEL(m_name_label)

	m_name = new QLineEdit(this);

	m_name_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_name_error_message)

	m_unit_label = new QLabel("Select the base unit for the beacon:", this);
	WRAPPED_HTML_LABEL(m_unit_label)

	m_unit = new QComboBox(this);
	auto units = mission.GetMission().BeaconUnits();
	QStringList unit_list;
	std::ranges::transform(units,
	                       std::back_inserter(unit_list),
	                       [](const std::string& unit)
	                       {
		                       return QString::fromStdString(unit);
	                       });
	m_unit->addItems(unit_list);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_name_label);
	m_layout->addWidget(m_name);
	m_layout->addWidget(m_name_error_message);
	m_layout->addWidget(m_unit_label);
	m_layout->addWidget(m_unit);
}

void BeaconInfoPage::initializePage()
{
	connect(m_name, &QLineEdit::textChanged, [&] { m_name_error_message->clear(); });
	QWizardPage::initializePage();
}

bool BeaconInfoPage::validatePage()
{
	if (m_name->text().isEmpty())
		return false;
	return true;
}
