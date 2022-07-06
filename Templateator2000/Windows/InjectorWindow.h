#pragma once

#include <QDialog>

#include "ui_InjectorWindow.h"

class InjectorWindow final : public QDialog
{
	Q_OBJECT

public:
	explicit InjectorWindow(const std::function<void(const std::vector<std::string>&)>& on_ok,
	                        const std::function<void()>& on_cancel,
	                        const std::vector<std::string>& already_injected = {},
	                        QWidget* parent = nullptr);

private slots:
	void onOkClicked();
	void onCancelClicked();

private:
	std::vector<std::string> m_injected_scripts;
	std::function<void(const std::vector<std::string>&)> m_on_ok;
	std::function<void()> m_on_cancel;
	Ui::InjectorWindowClass m_ui = {};
};
