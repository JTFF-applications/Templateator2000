#include <QInputDialog>
#include <QMessageBox>

#include "Windows/Wizards/WizardMacros.h"
#include "Windows/Wizards/Atis/AtisWizard.h"
#include "Windows/Wizards/Atis/Pages/AtisRadioPage.h"

QRegularExpressionValidator* AtisRadioPage::m_frequency_validator = new QRegularExpressionValidator(
	QRegularExpression("^[1-3][0-9]{2}[.][0-9](00|25|50|75)$"));

AtisRadioPage::AtisRadioPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle("Atis - Tower Frequencies");

	m_label = new QLabel("Here, you can set the airport tower frequencies the atis will say in his message.", this);
	WRAPPED_HTML_LABEL(m_label)

	m_frequencies = new QListWidget(this);
	m_add_fq_btn = new QPushButton("+", this);
	m_rm_fq_btn = new QPushButton("-", this);

	connect(m_add_fq_btn, &QPushButton::clicked, this, &AtisRadioPage::addRadioFrequency);
	connect(m_rm_fq_btn, &QPushButton::clicked, this, &AtisRadioPage::rmRadioFrequency);

	m_layout = new QGridLayout(this);
	m_layout->addWidget(m_label, 0, 0, 1, 2);
	m_layout->addWidget(m_frequencies, 1, 0, 1, 2);
	m_layout->addWidget(m_add_fq_btn, 2, 0, 1, 1);
	m_layout->addWidget(m_rm_fq_btn, 2, 1, 1, 1);
	setLayout(m_layout);
}

std::vector<std::string> AtisRadioPage::Frequencies() const
{
	std::vector<std::string> res;
	for (size_t i = 0; i < m_frequencies->count(); i++)
	{
		const auto& item = m_frequencies->item(i);
		res.push_back(item->text().toStdString());
	}
	return res;
}

int AtisRadioPage::nextId() const
{
	return reinterpret_cast<AtisWizard*>(wizard())->NextOption(AtisWizard::RadioPage);
}

void AtisRadioPage::addRadioFrequency()
{
	bool is_ok = false;
	QString res = QInputDialog::getText(this,
	                                    "Atis - Tower Frequencies",
	                                    "Enter Tower Frequency : ",
	                                    QLineEdit::Normal,
	                                    "",
	                                    &is_ok);

	if (is_ok && !res.isEmpty())
	{
		int pos = 0;
		if (m_frequency_validator->validate(res, pos) != QValidator::Acceptable)
		{
			LOG_INFO("Error in AtisWizard::AtisRadioPage::addRadioFrequency: {} is not a valid frequency !",
			         res.toStdString());
			QMessageBox::warning(this, "Error", "Invalid frequency !");
			return;
		}
		m_frequencies->addItem(res);
		LOG_INFO("Tower frequency {} added to atis !", res.toStdString());
	}
}

void AtisRadioPage::rmRadioFrequency() const
{
	qDeleteAll(m_frequencies->selectedItems());
	LOG_INFO("Selected tower frequencies removed from atis !");
}
