#pragma once

#include <QDialog>

#include "Models/Carrier.h"
#include "Utilities/DCS/Group.h"
#include "ui_CarrierWindow.h"

class CarrierWindow final : public QDialog
{
	Q_OBJECT

public:
	explicit CarrierWindow(QWidget* parent,
	                       const std::map<const std::string, const std::vector<Group>>& mission_data,
	                       const std::function<void(Carrier)>& on_ok,
	                       const std::function<void()>& on_cancel);
	virtual ~CarrierWindow() override;

	CarrierWindow(const CarrierWindow&) = delete;
	CarrierWindow(CarrierWindow&&) = delete;
	CarrierWindow& operator=(const CarrierWindow&) = delete;
	CarrierWindow& operator=(CarrierWindow&&) = delete;

	void SetCarrier(const Carrier& carrier) const;

private slots:
	void onRecoveryAddClicked();
	void onRecoveryRemoveClicked() const;

	void onOkClicked();
	void onCancelClicked();

private:
	const std::map<const std::string, const std::vector<Group>> m_mission_data;
	std::function<void(Carrier)> m_on_ok;
	std::function<void()> m_on_cancel;
	Ui::CarrierWindowClass m_ui = {};
};
