#pragma once

#include <filesystem>

#include "Models/Atis.h"
#include "Models/Awacs.h"
#include "Models/Beacon.h"
#include "Models/Carrier.h"
#include "Models/Tanker.h"
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
	void Save();
	void SaveAs(std::filesystem::path new_path);
	void ManageScripts();
	void UpdateScripts();

	[[nodiscard]] const bool& IsInitialized() const { return m_initialized; }
	[[nodiscard]] const bool& IsSaved() const { return m_is_saved; }

	[[nodiscard]] const std::string GetMissionName() const { return m_path.filename().string(); }
	[[nodiscard]] const DcsMission GetMission() const { return m_dcs_mission; }
	[[nodiscard]] const Scripts& GetScripts() const { return m_scripts; }
	[[nodiscard]] const std::map<const std::string, const std::vector<Group>> GetMissionGroups() const;

	// Tankers
	[[nodiscard]] const Tanker& GetTanker(const std::string& label) const;
	void AddTanker(const Tanker& tanker);
	void ModifyTanker(const Tanker& old_tanker, const Tanker& new_tanker);
	void RemoveTanker(const std::string& label);

	// Atis
	[[nodiscard]] const Atis& GetAtis(const std::string& label) const;
	void AddAtis(const Atis& atis);
	void ModifyAtis(const Atis& old_atis, const Atis& new_atis);
	void RemoveAtis(const std::string& label);

	// Beacons
	[[nodiscard]] const Beacon& GetBeacon(const std::string& label) const;
	void AddBeacon(const Beacon& beacon);
	void ModifyBeacon(const Beacon& old_beacon, const Beacon& new_beacon);
	void RemoveBeacon(const std::string& label);

	// Carriers
	[[nodiscard]] const Carrier& GetCarrier(const std::string& label) const;
	void AddCarrier(const Carrier& carrier);
	void ModifyCarrier(const Carrier& old_carrier, const Carrier& new_carrier);
	void RemoveCarrier(const std::string& label);

	// Awacs
	[[nodiscard]] const Awacs& GetAwacs(const std::string& label) const;
	void AddAwacs(const Awacs& awacs);
	void ModifyAwacs(const Awacs& old_awacs, const Awacs& new_awacs);
	void RemoveAwacs(const std::string& label);

private:
	std::filesystem::path m_path, m_mission_path;
	bool m_initialized, m_is_saved;

	DcsMission m_dcs_mission;
	Scripts m_scripts;
};
