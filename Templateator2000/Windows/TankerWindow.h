#pragma once

#include <vector>

#include "Models/Tanker.h"
#include "Utilities/DCS/Group.h"
#include "ui_TankerWindow.h"

class TankerWindow final : public QDialog
{
	Q_OBJECT

public:
	TankerWindow(QWidget* parent,
	             const std::map<const std::string, const std::vector<Group>>& mission_data,
	             const std::function<void(Tanker)>& on_ok,
	             const std::function<void()>& on_cancel);
	virtual ~TankerWindow() override;

	TankerWindow(const TankerWindow&) = delete;
	TankerWindow(TankerWindow&&) = delete;
	TankerWindow& operator=(const TankerWindow&) = delete;
	TankerWindow& operator=(TankerWindow&&) = delete;

	void SetTanker(const Tanker& tk) const;

private slots:
	void onOkClicked();
	void onCancelClicked();

private:
	const std::map<const std::string, const std::vector<Group>> m_mission_data;
	std::function<void(Tanker)> m_on_ok;
	std::function<void()> m_on_cancel;
	Ui::TankerWindow m_ui = {};
};
