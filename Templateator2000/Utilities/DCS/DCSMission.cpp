#include <libzippp/libzippp.h>
#include <nlohmann/json.hpp>

#include "Utilities/Log.h"
#include "Utilities/LUA/Lua.h"
#include "Utilities/DCS/DCSMission.h"

const std::map<Coalition::Side, const char*> DcsMission::s_coalitions = {
	{Coalition::Red, "red"}, {Coalition::Blue, "blue"}, {Coalition::Neutral, "neutrals"}
};

bool DcsMission::IsValidMission(const std::filesystem::path& path)
{
	libzippp::ZipArchive archive(path.string());
	if (!archive.open())
		return false;

	const libzippp::ZipEntry mission_file = archive.getEntry("mission");
	if (archive.close() != LIBZIPPP_OK)
		throw std::exception("Failed to close and save temporary mission !");
	if (!mission_file.isFile())
		return false;
	return true;
}

DcsMission::DcsMission()
	: m_initialized(false)
{
}

DcsMission::DcsMission(const std::filesystem::path& path)
{
	if (!IsValidMission(path))
		throw std::exception(std::format("Invalid mission {} !", path.string()).c_str());

	m_initialized = true;
	m_path = path;

	load();
}

void DcsMission::Init(const std::filesystem::path& path)
{
	if (!IsValidMission(path))
		throw std::exception(std::format("Invalid mission {} !", path.string()).c_str());

	m_path = path;

	m_map.clear();
	m_helicopters.clear();
	m_planes.clear();
	m_statics.clear();
	m_ships.clear();
	m_vehicles.clear();

	m_initialized = true;
	load();
}

const std::vector<std::string> DcsMission::Carriers(const Coalition::Side& coalition) const
{
	std::vector<std::string> res;
	for (const auto& group : m_ships)
		for (const auto& unit : group.Units())
			if (unit.Coalition() == coalition)
				if (unit.Name().find("CV") != std::string::npos)
					res.push_back(unit.Name());
	return res;
}

const std::vector<std::string> DcsMission::Escorts(const Coalition::Side& coalition) const
{
	std::vector<std::string> res;
	for (const auto& group : m_planes)
		for (const auto& unit : group.Units())
			if (unit.Coalition() == coalition)
				res.push_back(unit.Name());
	return res;
}

const std::vector<std::string> DcsMission::Tankers(const Coalition::Side& coalition) const
{
	std::vector<std::string> res;
	for (const auto& group : m_planes)
		for (const auto& unit : group.Units())
			if (unit.Coalition() == coalition)
			{
				const std::string& type = unit.Type();
				if (type == "KC130" || type == "KC-135" || type == "KC135MPRS" || type == "S-3B Tanker" || type ==
				    "KC_10_Extender" || type == "KC_10_Extender_D")
					res.push_back(group.Name());
			}
	return res;
}

const std::vector<std::string> DcsMission::Awacs(const Coalition::Side& coalition) const
{
	std::vector<std::string> res;
	for (const auto& group : m_planes)
		for (const auto& unit : group.Units())
			if (unit.Coalition() == coalition)
			{
				const std::string& type = unit.Type();
				if (type == "E-2C" || type == "E-3A")
					res.push_back(unit.Name());
			}
	return res;
}

const std::vector<std::string> DcsMission::OrbitUnits(const Coalition::Side& coalition) const
{
	std::vector<std::string> res;
	for (const auto& group : m_ships)
		for (const auto& unit : group.Units())
			if (unit.Coalition() == coalition)
				res.push_back(unit.Name());

	for (const auto& group : m_vehicles)
		for (const auto& unit : group.Units())
			if (unit.Coalition() == coalition)
				res.push_back(unit.Name());
	return res;
}

const std::vector<std::string> DcsMission::BeaconUnits() const
{
	std::vector<std::string> res;
	for (const auto& group : m_vehicles)
		for (const auto& unit : group.Units())
			res.push_back(unit.Name());
	return res;
}

void DcsMission::load()
{
	if (!m_initialized)
		return;

	libzippp::ZipArchive archive(m_path.string());
	if (!archive.open())
		throw std::exception("Failed to open temporary mission !");
	libzippp::ZipEntry mission_file = archive.getEntry("mission");

	try
	{
		json::json json = Lua::JsonFromLua(mission_file.readAsText(), "mission");
		for (const auto& [coalition_enum, coalition_string] : s_coalitions)
			for (const auto& country : json["coalition"][coalition_string]["country"])
			{
				// Statics
				if (country.contains("static") && country.at("static").contains("group"))
					for (const auto& static_group : country["static"]["group"])
					{
						std::vector<Unit> static_units;
						for (const auto& static_unit : static_group["units"])
						{
							const std::string unit_type = static_unit["type"].get<std::string>();
							const std::string unit_name = static_unit["name"].get<std::string>();
							static_units.emplace_back(unit_type, unit_name, "static", coalition_enum);
						}
						const std::string group_name = static_group["name"].get<std::string>();
						m_statics.emplace_back(group_name, static_units);
					}

				// Planes
				if (country.contains("plane") && country.at("plane").contains("group"))
					for (const auto& plane_group : country["plane"]["group"])
					{
						std::vector<Unit> plane_units;
						for (const auto& plane_unit : plane_group["units"])
						{
							const std::string unit_type = plane_unit["type"].get<std::string>();
							const std::string unit_name = plane_unit["name"].get<std::string>();
							const std::string unit_skill = plane_unit["skill"].get<std::string>();
							plane_units.emplace_back(unit_type, unit_name, unit_skill, coalition_enum);
						}
						const std::string group_name = plane_group["name"].get<std::string>();
						m_planes.emplace_back(group_name, plane_units);
					}

				// Helicopters
				if (country.contains("helicopter") && country.at("helicopter").contains("group"))
					for (const auto& helicopter_group : country["helicopter"]["group"])
					{
						std::vector<Unit> helicopter_units;
						for (const auto& helicopter_unit : helicopter_group["units"])
						{
							const std::string unit_type = helicopter_unit["type"].get<std::string>();
							const std::string unit_name = helicopter_unit["name"].get<std::string>();
							const std::string unit_skill = helicopter_unit["skill"].get<std::string>();
							helicopter_units.emplace_back(unit_type, unit_name, unit_skill, coalition_enum);
						}
						const std::string group_name = helicopter_group["name"].get<std::string>();
						m_helicopters.emplace_back(group_name, helicopter_units);
					}

				// Vehicle
				if (country.contains("vehicle") && country.at("vehicle").contains("group"))
					for (const auto& vehicle_group : country["vehicle"]["group"])
					{
						std::vector<Unit> vehicle_units;
						for (const auto& vehicle_unit : vehicle_group["units"])
						{
							const std::string unit_type = vehicle_unit["type"].get<std::string>();
							const std::string unit_name = vehicle_unit["name"].get<std::string>();
							const std::string unit_skill = vehicle_unit["skill"].get<std::string>();
							vehicle_units.emplace_back(unit_type, unit_name, unit_skill, coalition_enum);
						}
						const std::string group_name = vehicle_group["name"].get<std::string>();
						m_vehicles.emplace_back(group_name, vehicle_units);
					}

				// Ships
				if (country.contains("ship") && country.at("ship").contains("group"))
					for (const auto& ship_group : country["ship"]["group"])
					{
						std::vector<Unit> ship_units;
						for (const auto& ship_unit : ship_group["units"])
						{
							const std::string unit_type = ship_unit["type"].get<std::string>();
							const std::string unit_name = ship_unit["name"].get<std::string>();
							const std::string unit_skill = ship_unit["skill"].get<std::string>();
							ship_units.emplace_back(unit_type, unit_name, unit_skill, coalition_enum);
						}
						const std::string group_name = ship_group["name"].get<std::string>();
						m_ships.emplace_back(group_name, ship_units);
					}
			}

		m_map = archive.getEntry("theatre").readAsText();

		if (archive.close() != LIBZIPPP_OK)
			throw std::exception("Failed to close and save temporary mission !");
	} catch (const std::exception& except)
	{
		archive.close();
		throw std::exception(std::format("Selected mission {} is invalid. Exception thrown : {}",
		                                 m_path.string(),
		                                 except.what()).c_str());
	}
}
