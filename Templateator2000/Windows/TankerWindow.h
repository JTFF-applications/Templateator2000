#pragma once

#include <QDialog>

#include <vector>

#include "Utilities/DCS/Group.h"
#include "Models/Tanker.h"
#include "ui_TankerWindow.h"

class TankerWindow : public QDialog
{
	Q_OBJECT

public:
	TankerWindow(QWidget* parent, const std::vector<Group>& groups, std::function<void(Tanker)> on_ok, std::function<void(void)> on_cancel);
	TankerWindow(QWidget* parent, const Tanker& tanker, const std::vector<Group>& groups, std::function<void(Tanker)> on_ok, std::function<void(void)> on_cancel);
	~TankerWindow() = default;

private slots:
	void on_ok_clicked();

private:
	const std::vector<Group> m_groups;
	std::function<void(Tanker)> m_onOk;
	std::function<void(void)> m_onCancel;
	Ui::TankerWindow m_ui;
};
