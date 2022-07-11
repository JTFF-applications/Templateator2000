#pragma once

#include <QDialog>

#include "Models/Atis.h"
#include "Utilities/DCS/Group.h"
#include "ui_AtisWindow.h"

class AtisWindow final : public QDialog
{
	Q_OBJECT

public:
	explicit AtisWindow(QWidget* parent,
	                    const std::map<const std::string, const std::vector<Group>>& mission_data,
	                    const std::function<void(Atis)>& on_ok,
	                    const std::function<void()>& on_cancel);
	virtual ~AtisWindow() override;

	AtisWindow(const AtisWindow&) = delete;
	AtisWindow(AtisWindow&&) = delete;
	AtisWindow& operator=(const AtisWindow&) = delete;
	AtisWindow& operator=(AtisWindow&&) = delete;

	void SetAtis(const Atis& atis) const;

private slots:
	void onOkClicked();
	void onCancelClicked();

	void onTowerFqAddClicked();
	void onTowerFqRemoveClicked() const;

private:
	const std::map<const std::string, const std::vector<Group>> m_mission_data;
	std::function<void(Atis)> m_on_ok;
	std::function<void()> m_on_cancel;
	Ui::AtisWindowClass m_ui = {};
};
