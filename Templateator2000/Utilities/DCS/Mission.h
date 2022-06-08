#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>
#include <vector>

#include "Utilities/DCS/Group.h"

namespace json = nlohmann;

class Mission
{
public:
	Mission() = default;
	~Mission() = default;

	void Get();
	void Load();

	const std::vector<Group> GetHelicopters() const { return m_helicopters; }
	const std::vector<Group> GetPlanes() const { return m_planes; }
	const std::vector<Group> GetStatics() const { return m_statics; }
	const std::vector<Group> GetVehicules() const { return m_vehicules; }
	const std::vector<Group> GetShips() const { return m_ships; }

private:
	const static std::vector<const char*> s_coalitions;
	std::filesystem::path m_path;

	std::vector<Group> m_helicopters;
	std::vector<Group> m_planes;
	std::vector<Group> m_statics;
	std::vector<Group> m_vehicules;
	std::vector<Group> m_ships;
};

// Group name : mission/coalition/blue/country/{1}/{helicopter/vehicule/ship/plane/static}/group/{1}/name
// Unit name  : mission/coalition/blue/country/{1}/{helicopter/vehicule/ship/plane/static}/group/{1}/units/{1}/name