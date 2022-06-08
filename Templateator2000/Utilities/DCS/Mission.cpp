#include <QFileDialog>
#include <QMessageBox>

#include <libzippp/libzippp.h>
#include <nlohmann/json.hpp>

#include "Utilities/LUA/Lua.h"
#include "Utilities/Log.h"
#include "Mission.h"

const std::vector<const char*> Mission::s_coalitions = { "red", "blue", "neutrals" };

void Mission::Get()
{
	std::string filename = QFileDialog::getOpenFileName(nullptr, "Open Mission", "", "Mission Files (*.miz)").toStdString();
	if (filename.empty())
		return;
	else if (!std::filesystem::exists(filename))
	{
		QMessageBox::critical(nullptr, "Error", "Selected file does not exist or is not readable.");
		LOG_ERROR("Selected mission {} is invalid.", filename);
		return;
	}

	m_path = filename;
	Load();
}

void Mission::Load()
{
	libzippp::ZipArchive archive(m_path.string());
	archive.open(libzippp::ZipArchive::ReadOnly);
	if (!archive.isOpen())
	{
		QMessageBox::critical(nullptr, "Error", "Selected file is not a valid mission.");
		LOG_ERROR("Selected mission {} is invalid. Cannot read as .zip.", m_path.string());
		return;
	}

	libzippp::ZipEntry mission_file = archive.getEntry("mission");
	if (!mission_file.isFile())
	{
		QMessageBox::critical(nullptr, "Error", "Selected file is not a valid mission.");
		LOG_ERROR("Selected mission {} is invalid. Cannot find mission file.", m_path.string());
		return;
	}

	try
	{
		json::json json = Lua::JsonFromString(mission_file.readAsText());
		for (const auto& coalition : s_coalitions)
		{
			for (const auto& country : json["coalition"][coalition]["country"])
			{
				// Statics
				if (country.contains("static") && country.at("static").contains("group"))
					for (const auto& static_group : country["static"]["group"])
					{
						std::vector<const Unit> static_units;
						for (const auto& static_unit : static_group["units"])
						{
							const std::string unit_type = static_unit["type"].get<std::string>();
							const std::string unit_name = static_unit["name"].get<std::string>();
							static_units.emplace_back(unit_type, unit_name);
						}
						const std::string group_name = static_group["name"].get<std::string>();
						m_statics.emplace_back(group_name, static_units);
					}

				// Planes
				if (country.contains("plane") && country.at("plane").contains("group"))
					for (const auto& plane_group : country["plane"]["group"])
					{
						std::vector<const Unit> plane_units;
						for (const auto& plane_unit : plane_group["units"])
						{
							const std::string unit_type = plane_unit["type"].get<std::string>();
							const std::string unit_name = plane_unit["name"].get<std::string>();
							plane_units.emplace_back(unit_type, unit_name);
						}
						const std::string group_name = plane_group["name"].get<std::string>();
						m_planes.emplace_back(group_name, plane_units);
					}

				// Helicopters
				if (country.contains("helicopter") && country.at("helicopter").contains("group"))
					for (const auto& helicopter_group : country["helicopter"]["group"])
					{
						std::vector<const Unit> helicopter_units;
						for (const auto& helicopter_unit : helicopter_group["units"])
						{
							const std::string unit_type = helicopter_unit["type"].get<std::string>();
							const std::string unit_name = helicopter_unit["name"].get<std::string>();
							helicopter_units.emplace_back(unit_type, unit_name);
						}
						const std::string group_name = helicopter_group["name"].get<std::string>();
						m_helicopters.emplace_back(group_name, helicopter_units);
					}

				// Vehicules
				if (country.contains("vehicle") && country.at("vehicle").contains("group"))
					for (const auto& vehicule_group : country["vehicle"]["group"])
					{
						std::vector<const Unit> vehicule_units;
						for (const auto& vehicule_unit : vehicule_group["units"])
						{
							const std::string unit_type = vehicule_unit["type"].get<std::string>();
							const std::string unit_name = vehicule_unit["name"].get<std::string>();
							vehicule_units.emplace_back(unit_type, unit_name);
						}
						const std::string group_name = vehicule_group["name"].get<std::string>();
						m_vehicules.emplace_back(group_name, vehicule_units);
					}

				// Ships
				if (country.contains("ship") && country.at("ship").contains("group"))
					for (const auto& ship_group : country["ship"]["group"])
					{
						std::vector<const Unit> ship_units;
						for (const auto& ship_unit : ship_group["units"])
						{
							const std::string unit_type = ship_unit["type"].get<std::string>();
							const std::string unit_name = ship_unit["name"].get<std::string>();
							ship_units.emplace_back(unit_type, unit_name);
						}
						const std::string group_name = ship_group["name"].get<std::string>();
						m_ships.emplace_back(group_name, ship_units);
					}
			}
		}
	}
	catch (const std::exception& except)
	{
		QMessageBox::critical(nullptr, "Error", "Selected file is not a valid mission.");
		LOG_ERROR("Selected mission {} is invalid. Exception thrown : {}", m_path.string(), except.what());
		return;
	}
}
