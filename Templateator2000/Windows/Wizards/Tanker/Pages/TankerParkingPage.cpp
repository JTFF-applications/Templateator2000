#include "Utilities/Moose.h"

#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerParkingPage.h"

TankerParkingPage::TankerParkingPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Tanker Parking Size/Position");

	m_parkings_label = new QLabel(
		"Select the size of the parking required for the tanker.<br>"
		R"(If you are not used to the options, please check the documentation <a href="http://google.com">here</a>:)",
		this);
	WRAPPED_HTML_LABEL(m_parkings_label)

	m_parkings_list = new QComboBox(this);
	m_parkings_list->addItems(Moose::GetQtParkings());

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_parkings_label);
	m_layout->addWidget(m_parkings_list);
	setLayout(m_layout);
}

int TankerParkingPage::nextId() const
{
	if (reinterpret_cast<TankerWizard*>(wizard())->Type() == Tanker::Type::Fixed)
		return TankerWizard::FlightPage;
	return TankerWizard::OnDemandFlightPage;
}
