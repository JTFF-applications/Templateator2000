#pragma once

#include <QDialog>

#include "Models/Beacon.h"
#include "Utilities/DCS/Group.h"
#include "ui_BeaconWindow.h"

class BeaconWindow final : public QDialog
{
	Q_OBJECT

public:
	explicit BeaconWindow(QWidget* parent,
	                      const std::map<const std::string, const std::vector<Group>>& mission_data,
	                      const std::function<void(Beacon)>& on_ok,
	                      const std::function<void()>& on_cancel);
	virtual ~BeaconWindow() override;

	BeaconWindow(const BeaconWindow&) = delete;
	BeaconWindow(BeaconWindow&&) = delete;
	BeaconWindow& operator=(const BeaconWindow&) = delete;
	BeaconWindow& operator=(BeaconWindow&&) = delete;

	void SetBeacon(const Beacon& beacon) const;

private slots:
	void onOkClicked();
	void onCancelClicked();

private:
	const std::map<const std::string, const std::vector<Group>> m_mission_data;
	std::function<void(Beacon)> m_on_ok;
	std::function<void()> m_on_cancel;
	Ui::BeaconWindowClass m_ui = {};
};
