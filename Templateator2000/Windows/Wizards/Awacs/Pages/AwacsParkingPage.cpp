#include "Utilities/Moose.h"

#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Awacs/AwacsWizard.h"
#include "Windows/Wizards/Awacs/Pages/AwacsParkingPage.h"

AwacsParkingPage::AwacsParkingPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Awacs Parking Size/Position");

	m_parkings_label = new QLabel(
		"Select the size of the parking required for the awacs.<br>"
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

int AwacsParkingPage::nextId() const
{
	return AwacsWizard::FlightPage;
}
