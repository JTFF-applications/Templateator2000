#include "Utilities/Moose.h"

#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Awacs/AwacsWizard.h"
#include "Windows/Wizards/Awacs/Pages/AwacsDeparturePage.h"

AwacsDeparturePage::AwacsDeparturePage(const Mission& mission, QWidget* parent)
	: QWizardPage(parent), m_mission(mission)
{
	setTitle("Awacs Departure Airbase/Unit");

	m_departure_label = new QLabel(
		R"(Select the departure airbase/unit for the awacs <a href="http://google.com">(more info)</a>:)",
		this);
	WRAPPED_HTML_LABEL(m_departure_label)

	m_departure_list = new QComboBox(this);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_departure_label);
	m_layout->addWidget(m_departure_list);
	setLayout(m_layout);
}

void AwacsDeparturePage::initializePage()
{
	QStringList airports;
	std::ranges::transform(Moose::GetAirbasesByMap(m_mission.GetMission().GetMap()),
	                       std::back_inserter(airports),
	                       [](const std::string& airport) { return QString::fromStdString(airport); });
	airports.sort(Qt::CaseInsensitive);

	QStringList carriers;
	std::ranges::transform(m_mission.GetMission().Carriers(reinterpret_cast<AwacsWizard*>(wizard())->Coalition()),
	                       std::back_inserter(carriers),
	                       [](const std::string& carrier) { return QString::fromStdString(carrier); });
	carriers.sort(Qt::CaseInsensitive);

	m_departure_list->addItems(airports);
	m_departure_list->addItems(carriers);

	emit init();

	QWizardPage::initializePage();
}

int AwacsDeparturePage::nextId() const
{
	const auto& ships = m_mission.GetMission().GetShips();
	if (std::ranges::find_if(ships,
	                         [&](const Group& group)
	                         {
		                         return std::ranges::find_if(group.Units(),
		                                                     [&](const Unit& unit)
		                                                     {
			                                                     return unit.Name() == m_departure_list
			                                                            ->currentText().toStdString();
		                                                     }) != group.Units().end();
	                         }) != ships.end())
		return AwacsWizard::CarrierPage;
	return AwacsWizard::ParkingPage;
}
