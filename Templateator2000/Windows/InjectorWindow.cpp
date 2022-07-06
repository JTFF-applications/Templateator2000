#include "Windows/InjectorWindow.h"

InjectorWindow::InjectorWindow(const std::function<void(const std::vector<std::string>&)>& on_ok,
                               const std::function<void()>& on_cancel,
                               const std::vector<std::string>& already_injected,
                               QWidget* parent)
	: QDialog(parent), m_injected_scripts(already_injected), m_on_ok(on_ok), m_on_cancel(on_cancel)
{
	m_ui.setupUi(this);

	connect(m_ui.ok_btn, &QPushButton::clicked, this, &InjectorWindow::onOkClicked);
	connect(m_ui.cancel_btn, &QPushButton::clicked, this, &InjectorWindow::onCancelClicked);

	for (const auto& script : m_injected_scripts)
	{
		const auto item = m_ui.checkboxes->findChild<QCheckBox*>(script.c_str());
		item->setChecked(true);
		item->setCheckable(false);
	}
}

void InjectorWindow::onOkClicked()
{
	m_injected_scripts.clear();
	for (int i = 0; i < m_ui.checkboxes->rowCount(); i++)
		for (int j = 0; j < m_ui.checkboxes->columnCount(); j++)
		{
			const auto checkbox = qobject_cast<QCheckBox*>(m_ui.checkboxes->itemAtPosition(i, j)->widget());
			if (checkbox->isChecked())
				m_injected_scripts.push_back(checkbox->objectName().toStdString());
		}
	close();
	std::invoke(m_on_ok, m_injected_scripts);
}

void InjectorWindow::onCancelClicked()
{
	close();
	std::invoke(m_on_cancel);
}
