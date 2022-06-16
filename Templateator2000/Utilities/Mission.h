#pragma once

#include <QStringList>

#include <filesystem>

#include "Utilities/DCS/DCSMission.h"
#include "Utilities/Scripting/Scripts.h"

class Mission
{
public:
	static QStringList DataToGroupName(const std::map<const std::string, const std::vector<Group>>& mission_data);
	static QStringList DataToUnitName(const std::map<const std::string, const std::vector<Group>>& mission_data);

public:
	Mission();
	explicit Mission(const std::filesystem::path& path, const std::filesystem::path& mission_path);

	void Init(const std::filesystem::path& path, const std::filesystem::path& mission_path);
	void Save() const;
	[[nodiscard]] const bool& IsInitialized() const { return m_initialized; }

	[[nodiscard]] const std::string GetMissionName() const { return m_path.filename().string(); }
	[[nodiscard]] const Scripts& GetScripts() const { return m_scripts; }
	[[nodiscard]] const std::map<const std::string, const std::vector<Group>> GetMissionGroups() const;

	// Tankers
	[[nodiscard]] const Tanker& GetTanker(const std::string& label) const;
	void AddTanker(const Tanker& tanker);
	void ModifyTanker(const Tanker& old_tanker, const Tanker& new_tanker);
	void RemoveTanker(const std::string& label);

private:
	std::filesystem::path m_path, m_mission_path;
	bool m_initialized;

	DcsMission m_dcs_mission;
	Scripts m_scripts;
};
