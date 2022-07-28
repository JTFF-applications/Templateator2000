#pragma once

#include "Models/Awacs.h"
#include "Utilities/DCS/Group.h"
#include "ui_AwacsWindow.h"

class AwacsWindow final : public QDialog
{
	Q_OBJECT

public:
	explicit AwacsWindow(QWidget* parent,
	                     const std::map<const std::string, const std::vector<Group>>& mission_data,
	                     const std::function<void(Awacs)>& on_ok,
	                     const std::function<void()>& on_cancel);
	virtual ~AwacsWindow() override;

	AwacsWindow(const AwacsWindow&) = delete;
	AwacsWindow(AwacsWindow&&) = delete;
	AwacsWindow& operator=(const AwacsWindow&) = delete;
	AwacsWindow& operator=(AwacsWindow&&) = delete;

	void SetAwacs(const Awacs& atis) const;

private slots:
	void onOkClicked();
	void onCancelClicked();

private:
	const std::map<const std::string, const std::vector<Group>> m_mission_data;
	std::function<void(Awacs)> m_on_ok;
	std::function<void()> m_on_cancel;
	Ui::AwacsWindowClass m_ui = {};
};
