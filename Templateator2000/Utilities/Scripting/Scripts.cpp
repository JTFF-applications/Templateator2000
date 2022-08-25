#include <fstream>

#include <libzippp/libzippp.h>

#include "Utilities/Log.h"
#include "Utilities/Moose.h"
#include "Utilities/LUA/Lua.h"
#include "Utilities/Scripting/Injector.h"
#include "Utilities/Scripting/Scripts.h"

Scripts::Scripts()
	: m_initialized(false)
{
}

Scripts::Scripts(const std::filesystem::path& path)
	: m_path(path), m_initialized(true)
{
	load();
}

void Scripts::Init(const std::filesystem::path& path)
{
	m_path = path;

	m_installed_scripts.clear();
	m_tankers.clear();
	m_atis.clear();
	m_beacons.clear();
	m_carriers.clear();
	m_awacs.clear();

	m_initialized = true;
	load();
}

void Scripts::Save() const
{
	if (!m_initialized)
		return;

	libzippp::ZipArchive archive(m_path.string());
	if (!archive.open(libzippp::ZipArchive::Write))
		throw std::exception("Failed to open mission for writing !");

	for (const auto& script : m_installed_scripts)
		if (script == "tankers" && !m_tankers.empty())
		{
			std::fstream fixed_file("temp/settings-tankers.lua", std::ios::out);
			std::fstream ondemand_file("temp/settings-ondemandtankers.lua", std::ios::out);

			json::json full_fixed_tanker_json = {};
			json::json full_ondemand_tanker_json = {};

			for (const auto& tanker : m_tankers)
				if (tanker.Type == Tanker::Type::Fixed)
					full_fixed_tanker_json += Tanker::ToJson(tanker, tanker.Type);
				else
					full_ondemand_tanker_json += Tanker::ToJson(tanker, tanker.Type);

			fixed_file << Lua::LuaFromJson(full_fixed_tanker_json, "TankersConfig");
			ondemand_file << Lua::LuaFromJson(full_ondemand_tanker_json, "OnDemandTankersConfig");

			fixed_file.close();
			ondemand_file.close();

			if (!archive.addFile("l10n/DEFAULT/settings-tankers.lua", "temp/settings-tankers.lua"))
				throw std::exception("Failed to write fixed tanker config file in mission !");
			if (!archive.addFile("l10n/DEFAULT/settings-ondemandtankers.lua", "temp/settings-ondemandtankers.lua"))
				throw std::exception("Failed to write on demand tanker config file in mission !");
		}
		else if (script == "atis" && !m_atis.empty())
		{
			std::fstream file("temp/settings-atis.lua", std::ios::out);
			json::json full_atis_json = {};
			for (const auto& atis : m_atis)
				full_atis_json += Atis::ToJson(atis);
			file << Lua::LuaFromJson(full_atis_json, "AtisConfig");
			file.close();

			if (!archive.addFile("l10n/DEFAULT/settings-atis.lua", "temp/settings-atis.lua"))
				throw std::exception("Failed to write atis config file in mission !");
		}
		else if (script == "beacons" && !m_beacons.empty())
		{
			std::fstream file("temp/settings-beacons.lua", std::ios::out);
			json::json full_beacons_json = {};
			for (const auto& beacon : m_beacons)
				full_beacons_json += Beacon::ToJson(beacon);
			file << Lua::LuaFromJson(full_beacons_json, "BeaconsConfig");
			file.close();

			if (!archive.addFile("l10n/DEFAULT/settings-beacons.lua", "temp/settings-beacons.lua"))
				throw std::exception("Failed to write beacons config file in mission !");
		}
		else if (script == "airboss" && !m_carriers.empty())
		{
			std::fstream file("temp/settings-airboss.lua", std::ios::out);
			json::json full_carriers_json = {};
			for (const auto& carrier : m_carriers)
				full_carriers_json += Carrier::ToJson(carrier);
			file << Lua::LuaFromJson(full_carriers_json, "AirBossConfig");
			file.close();

			if (!archive.addFile("l10n/DEFAULT/settings-airboss.lua", "temp/settings-airboss.lua"))
				throw std::exception("Failed to write airboss config file in mission !");
		}
		else if (script == "awacs" && !m_awacs.empty())
		{
			std::fstream file("temp/settings-awacs.lua", std::ios::out);
			json::json full_awacs_json = {};
			for (const auto& awacs : m_awacs)
				full_awacs_json += Awacs::ToJson(awacs);
			file << Lua::LuaFromJson(full_awacs_json, "AwacsConfig");
			file.close();

			if (!archive.addFile("l10n/DEFAULT/settings-awacs.lua", "temp/settings-awacs.lua"))
				throw std::exception("Failed to write awacs config file in mission !");
		}
	if (archive.close() != LIBZIPPP_OK)
		throw std::exception("Failed to close and save temporary mission !");
}

void Scripts::Manage()
{
	libzippp::ZipArchive archive(m_path.string());
	if (!archive.open(libzippp::ZipArchive::Write))
		throw std::exception("Failed to open mission for writing !");

	m_installed_scripts = installedScripts(archive.getEntry("l10n/DEFAULT/injected_scripts.lua").readAsText());
	Injector::ManageScripts(archive, m_installed_scripts);

	if (archive.close() != LIBZIPPP_OK)
		throw std::exception("Failed to close and save temporary mission !");
}

void Scripts::load()
{
	if (!m_initialized)
		return;

	libzippp::ZipArchive archive(m_path.string());
	if (!archive.open(libzippp::ZipArchive::Write))
		throw std::exception("Failed to open temporary mission !");

	std::vector<libzippp::ZipEntry> entries = archive.getEntries();
	bool scripts_injected = false;
	std::ranges::for_each(entries,
	                      [&](const libzippp::ZipEntry& entry)
	                      {
		                      auto name = entry.getName();
		                      if (entry.getName() == "l10n/DEFAULT/injected_scripts.lua")
			                      scripts_injected = true;
	                      });

	if (!scripts_injected)
	{
		Injector::ManageScripts(archive, m_installed_scripts);

		// Save changes when scripts are injected before continue
		if (archive.close() != LIBZIPPP_OK)
			throw std::exception("Failed to close and save temporary mission !");
		if (!archive.open(libzippp::ZipArchive::Write))
			throw std::exception("Failed to open temporary mission !");
	}

	m_installed_scripts = installedScripts(archive.getEntry("l10n/DEFAULT/injected_scripts.lua").readAsText());

	// ReSharper disable StringLiteralTypo
	for (const auto& file : entries)
	{
		if (!file.getName().starts_with("l10n/DEFAULT/"))
			continue;

		const std::string name = file.getName();
		if (name.find("settings-tankers") != std::string::npos)
		{
			json::json tankers = Lua::JsonFromLua(file.readAsText(), "TankersConfig");
			for (const auto& tanker : tankers)
				m_tankers.push_back(Tanker::FromJson(tanker, Tanker::Type::Fixed));
		}
		else if (name.find("settings-ondemandtankers") != std::string::npos)
		{
			json::json tankers = Lua::JsonFromLua(file.readAsText(), "OnDemandTankersConfig");
			for (const auto& tanker : tankers)
				m_tankers.push_back(Tanker::FromJson(tanker, Tanker::Type::OnDemand));
		}
		else if (name.find("settings-atis") != std::string::npos)
		{
			json::json atis_list = Lua::JsonFromLua(file.readAsText(), "AtisConfig");
			for (const auto& atis : atis_list)
				m_atis.push_back(Atis::FromJson(atis));
		}
		else if (name.find("settings-beacons") != std::string::npos)
		{
			json::json beacons = Lua::JsonFromLua(file.readAsText(), "BeaconsConfig");
			for (const auto& beacon : beacons)
				m_beacons.push_back(Beacon::FromJson(beacon));
		}
		else if (name.find("settings-airboss") != std::string::npos)
		{
			json::json carriers = Lua::JsonFromLua(file.readAsText(), "AirBossConfig");
			for (const auto& carrier : carriers)
				m_carriers.push_back(Carrier::FromJson(carrier));
		}
		else if (name.find("settings-awacs") != std::string::npos)
		{
			json::json awacs_list = Lua::JsonFromLua(file.readAsText(), "AwacsConfig");
			for (const auto& awacs : awacs_list)
				m_awacs.push_back(Awacs::FromJson(awacs));
		}
	}

	if (archive.close() != LIBZIPPP_OK)
		throw std::exception("Failed to close and save temporary mission !");
	// ReSharper restore StringLiteralTypo
}

const std::vector<std::string> Scripts::installedScripts(const std::string& file_data)
{
	std::vector<std::string> scripts;

	json::json scripts_installed = Lua::JsonFromLua(file_data, "InjectedScripts");
	for (const auto& pair : scripts_installed.items())
		scripts.push_back(pair.value());

	return scripts;
}
