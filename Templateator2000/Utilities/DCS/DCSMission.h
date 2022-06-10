#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>
#include <vector>

#include "Utilities/DCS/Group.h"

namespace json = nlohmann;

class DCSMission
{
public:
	static bool IsValidMission(const std::filesystem::path& path);

public:
	DCSMission();
	DCSMission(const std::filesystem::path& path);
	~DCSMission() = default;

	void Init(const std::filesystem::path& path);

	const std::vector<Group> GetHelicopters() const { return m_helicopters; }
	const std::vector<Group> GetPlanes() const { return m_planes; }
	const std::vector<Group> GetStatics() const { return m_statics; }
	const std::vector<Group> GetVehicules() const { return m_vehicules; }
	const std::vector<Group> GetShips() const { return m_ships; }

private:
	void Load();

private:
	const static std::vector<const char*> s_coalitions;
	std::filesystem::path m_path;
	bool m_initialized;

	std::vector<Group> m_helicopters;
	std::vector<Group> m_planes;
	std::vector<Group> m_statics;
	std::vector<Group> m_vehicules;
	std::vector<Group> m_ships;
};

// Group name : mission/coalition/blue/country/{1}/{helicopter/vehicule/ship/plane/static}/group/{1}/name
// Unit name  : mission/coalition/blue/country/{1}/{helicopter/vehicule/ship/plane/static}/group/{1}/units/{1}/name