#pragma once

#include <filesystem>
#include <vector>

#include <nlohmann/json.hpp>

#include "Utilities/Coalition.h"
#include "Utilities/DCS/Group.h"

namespace json = nlohmann;

class DcsMission
{
public:
	static bool IsValidMission(const std::filesystem::path& path);

public:
	DcsMission();
	explicit DcsMission(const std::filesystem::path& path);

	void Init(const std::filesystem::path& path);
	[[nodiscard]] const std::string GetMap() const { return m_map; }
	[[nodiscard]] const std::vector<std::string> Carriers(const Coalition::Side& coalition) const;
	[[nodiscard]] const std::vector<std::string> Escorts(const Coalition::Side& coalition) const;
	[[nodiscard]] const std::vector<std::string> Tankers(const Coalition::Side& coalition) const;
	[[nodiscard]] const std::vector<std::string> OrbitUnits(const Coalition::Side& coalition) const;
	[[nodiscard]] const std::vector<std::string> BeaconUnits() const;

	[[nodiscard]] const std::vector<Group> GetHelicopters() const { return m_helicopters; }
	[[nodiscard]] const std::vector<Group> GetPlanes() const { return m_planes; }
	[[nodiscard]] const std::vector<Group> GetStatics() const { return m_statics; }
	[[nodiscard]] const std::vector<Group> GetVehicles() const { return m_vehicles; }
	[[nodiscard]] const std::vector<Group> GetShips() const { return m_ships; }

private:
	void load();

private:
	const static std::map<Coalition::Side, const char*> s_coalitions;
	std::filesystem::path m_path;
	std::string m_map;
	bool m_initialized;

	std::vector<Group> m_helicopters;
	std::vector<Group> m_planes;
	std::vector<Group> m_statics;
	std::vector<Group> m_vehicles;
	std::vector<Group> m_ships;
};
