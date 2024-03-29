﻿#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Tanker/TankerWizard.h"
#include "Windows/Wizards/Tanker/Pages/TankerOnDemandFlightPage.h"

TankerOnDemandFlightPage::TankerOnDemandFlightPage(const Mission& mission, QWidget* parent)
	: QWizardPage(parent), m_mission(mission)
{
	setTitle("On Demand Tanker Default Information");

	m_name_label = new QLabel("Select the tanker name:", this);
	WRAPPED_HTML_LABEL(m_name_label)

	m_name = new QLineEdit(this);

	m_name_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_name_error_message)

	m_yes_btn = new QRadioButton("Enter default information.", this);
	m_no_btn = new QRadioButton("Do not enter default information.", this);

	m_default_error_message = new QLabel(this);
	LABEL_ERROR_MSG(m_default_error_message)

	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_name_label);
	m_layout->addWidget(m_name);
	m_layout->addWidget(m_name_error_message);
	m_layout->addWidget(m_yes_btn);
	m_layout->addWidget(m_no_btn);
	m_layout->addWidget(m_default_error_message);
	setLayout(m_layout);
}

void TankerOnDemandFlightPage::initializePage()
{
	connect(m_name, &QLineEdit::textChanged, [&] { m_name_error_message->clear(); });
	connect(m_yes_btn, &QRadioButton::clicked, [&] { m_default_error_message->clear(); });
	connect(m_no_btn, &QRadioButton::clicked, [&] { m_default_error_message->clear(); });
	QWizardPage::initializePage();
}

bool TankerOnDemandFlightPage::validatePage()
{
	bool res = true;
	const auto& on_demand_tankers = m_mission.GetScripts().GetOnDemandTankers();

	if (m_name->text().length() == 0)
	{
		res = false;
		m_name_error_message->setText("Name cannot be empty.");
	}
	else if (std::ranges::find_if(on_demand_tankers,
	                              [&](const Tanker& tk)
	                              {
		                              return tk.Name == m_name->text().toStdString() && tk.Name != reinterpret_cast<
			                                     TankerWizard*>(wizard())->InternalGetTanker().Name;
	                              }) != on_demand_tankers.cend())
	{
		res = false;
		m_name_error_message->setText("You can't select an already used name.");
	}
	if (!m_yes_btn->isChecked() && !m_no_btn->isChecked())
	{
		res = false;
		m_default_error_message->setText("You need to select a an option !");
	}
	return res;
}

int TankerOnDemandFlightPage::nextId() const
{
	if (m_yes_btn->isChecked())
		return TankerWizard::FlightPage;
	return TankerWizard::MissionPage;
}
