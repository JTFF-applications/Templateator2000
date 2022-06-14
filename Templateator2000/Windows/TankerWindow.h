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
	TankerWindow(QWidget* parent, const std::map<const std::string, const std::vector<Group>>& mission_data, std::function<void(Tanker)> on_ok, std::function<void(void)> on_cancel);
	~TankerWindow();

	void SetTanker(const Tanker& tk);

private slots:
	void on_ok_clicked();
	void on_cancel_clicked();

private:
	const std::map<const std::string, const std::vector<Group>> m_missionData;
	std::function<void(Tanker)> m_onOk;
	std::function<void(void)> m_onCancel;
	Ui::TankerWindow m_ui;
};
