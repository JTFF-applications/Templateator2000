#include <QIntValidator>

#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerCarrierPage.h"

TankerCarrierPage::TankerCarrierPage(QWidget* parent)
{
	setTitle("Tanker Carrier Information");

	m_modex_label = new QLabel(
		"Select the the modex the tanker wil use in naval operations.",
		this);
	WRAPPED_HTML_LABEL(m_modex_label)

	m_modex = new QLineEdit(this);
	m_modex->setValidator(new QIntValidator(100, 999, m_modex));

	m_airboss_label = new QLabel("Select if your tanker is landing at a carrier controlled by airboss script.",
	                             this);
	WRAPPED_HTML_LABEL(m_airboss_label)

	m_airboss_yes = new QRadioButton("Yes, the carrier is controlled by airboss.", this);
	m_airboss_no = new QRadioButton("No, the carrier is not controlled by airboss.", this);

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_modex_label);
	m_layout->addWidget(m_modex);
	m_layout->addWidget(m_airboss_label);
	m_layout->addWidget(m_airboss_yes);
	m_layout->addWidget(m_airboss_no);
	setLayout(m_layout);
}

int TankerCarrierPage::nextId() const
{
	if (reinterpret_cast<TankerWizard*>(wizard())->Type() == Tanker::Type::Fixed)
		return TankerWizard::FlightPage;
	return TankerWizard::OnDemandFlightPage;
}
