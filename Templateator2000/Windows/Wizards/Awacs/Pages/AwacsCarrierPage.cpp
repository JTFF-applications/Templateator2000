#include "Utilities/Validators/QRealIntValidator.h"
#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Awacs/AwacsWizard.h"
#include "Windows/Wizards/Awacs/Pages/AwacsCarrierPage.h"

AwacsCarrierPage::AwacsCarrierPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Awacs Carrier Information");

	m_modex_label = new QLabel(
		"Select the the modex the awacs wil use in naval operations.",
		this);
	WRAPPED_HTML_LABEL(m_modex_label)

	m_modex = new QLineEdit(this);
	m_modex->setValidator(new QRealIntValidator(100, 999, m_modex));

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
