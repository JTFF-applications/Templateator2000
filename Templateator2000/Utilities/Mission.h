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
	Mission(const std::filesystem::path& path);
	~Mission() = default;

	void Init(const std::filesystem::path& path);
	const bool& IsInitialized() const { return m_initialized; }

	const std::string GetMissionName() const { return m_path.filename().string(); }
	const Scripts& GetScripts() const { return m_scripts; }
	const std::map<const std::string, const std::vector<Group>> GetMissionGroups();

	const Tanker& GetTanker(const std::string& label) const;
	void AddTanker(const Tanker& tanker);
	void ModifyTanker(const Tanker& old_tanker, const Tanker& new_tanker);
	void RemoveTanker(const std::string& label);

private:
	std::filesystem::path m_path;
	bool m_initialized;

	DCSMission m_dcsMission;
	Scripts m_scripts;
};
