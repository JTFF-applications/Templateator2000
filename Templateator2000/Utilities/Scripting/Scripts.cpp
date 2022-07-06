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
	if (m_initialized)
		return;

	m_initialized = true;
	m_path = path;

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
			std::fstream file("temp/settings-tankers.lua", std::ios::out);
			json::json full_tanker_json = {};
			for (const auto& tanker : m_tankers)
				full_tanker_json += Tanker::ToJson(tanker);
			file << Lua::LuaFromJson(full_tanker_json, "TankersConfig");
			file.close();

			if (!archive.addFile("l10n/DEFAULT/settings-tankers.lua", "temp/settings-tankers.lua"))
				throw std::exception("Failed to write tanker config file in mission !");
		}
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
		                      if (entry.getName() == "injected_scripts.lua")
			                      scripts_injected = true;
	                      });

	if (!scripts_injected)
	{
		Injector::InjectScripts(archive, m_installed_scripts);

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
			{
				const bool is_escorted = tanker.contains("escortgroupname");
				const bool is_default_tacan_band = !tanker["tacan"].contains("band");
				Tanker tanker_object = {
					.Type = Tanker::Type::Fixed,
					.Coalition = Coalition::FromDcsCoalition(tanker["benefit_coalition"].get<int>()),
					.AutoRespawn = tanker["autorespawn"].get<bool>(),
					.AirbossRecovery = tanker["airboss_recovery"].get<bool>(),
					.PatternUnit = tanker["patternUnit"].get<std::string>(),
					.DepartureBase = Moose::GetMooseAirbaseFromName(tanker["baseUnit"].get<std::string>()),
					.TerminalType = Moose::GetMooseTerminalFromNumber(tanker["terminalType"].get<int>()),
					.GroupName = tanker["groupName"].get<std::string>(),
					.EscortGroupName = is_escorted ? tanker["escortgroupname"].get<std::string>() : "",
					.Frequency = std::format("{:.3f}", tanker["freq"].get<float>()),
					.MaxMissionDuration = tanker["missionmaxduration"].get<int>(),
					.Altitude = tanker["altitude"].get<int>(),
					.Speed = tanker["speed"].get<int>(),
					.FuelWarningLevel = tanker["fuelwarninglevel"].get<int>(),
					.Modex = tanker["modex"].get<int>(),
					.TacanChannel = tanker["tacan"]["channel"].get<int>(),
					.TacanBand = is_default_tacan_band ? "Y" : tanker["tacan"]["band"].get<std::string>(),
					.TacanMorse = tanker["tacan"]["morse"].get<std::string>(),
					.RacetrackFront = tanker["racetrack"]["front"].get<int>(),
					.RacetrackBack = tanker["racetrack"]["back"].get<int>(),
					.Callsign = Moose::GetMooseCallsignFromNumber(tanker["callsign"]["name"].get<int>(),
					                                              "CALLSIGN.Tanker"),
					.CallsignNb = tanker["callsign"]["number"].get<int>()
				};
				m_tankers.push_back(tanker_object);
			}
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
