#include <QMessageBox>

#include <libzippp/libzippp.h>

#include "Utilities/LUA/Lua.h"
#include "Utilities/Log.h"
#include "Scripts.h"

Scripts::Scripts()
	: m_initialized(false)
{
}

Scripts::Scripts(const std::filesystem::path& path)
	: m_initialized(true), m_path(path)
{
	Load();
}

void Scripts::Init(const std::filesystem::path& path)
{
	if (m_initialized)
		return;

	m_initialized = true;
	m_path = path;

	Load();
}

void Scripts::Load()
{
	if (!m_initialized)
		return;

	libzippp::ZipArchive archive(m_path.string());
	archive.open(libzippp::ZipArchive::ReadOnly);

	std::vector<libzippp::ZipEntry> entries = archive.getEntries();
	bool scripts_injected = false;
	std::for_each(entries.begin(), entries.end(), [&](libzippp::ZipEntry entry) {
		if (entry.getName() == "scripts.txt")
			scripts_injected = true;
		});

	if (!scripts_injected)
		throw std::exception("JTFF Scripts are not installed in this mission !");

	std::string scripts_installed = archive.getEntry("scripts.txt").readAsText();

	std::string scripts_installed_line;
	std::istringstream scripts_installed_stream(scripts_installed);
	while (std::getline(scripts_installed_stream, scripts_installed_line))
		m_scripts_installed.push_back(scripts_installed_line);

	for (const auto& file : entries)
	{
		if (!file.getName().starts_with("l10n/DEFAULT/"))
			continue;

		const std::string name = file.getName();
		if (name.find("settings-tankers") != std::string::npos)
		{
			json::json tankers = Lua::JsonFromConfigFile(file.readAsText(), "TankersConfig");
			for (const auto& tanker : tankers)
			{
				Tanker tanker_object = {
					.Type = Tanker::Type::Fixed,
					.Coalition = tanker["benefit_coalition"] == "coalition.side.BLUE" ? Coalition::Blue : tanker["benefit_coalition"] == "coalition.side.RED" ? Coalition::Red : Coalition::Neutral,
					.PatternUnit = tanker["patternUnit"].get<std::string>(),
					.DepartureBase = tanker["baseUnit"].get<std::string>(),
					.Callsign = tanker["callsign"]["name"].get<std::string>(),
					.Frequency = std::format("{:.3f}", tanker["freq"].get<float>()),
					.TacanMorse = tanker["tacan"]["morse"].get<std::string>(),
					.AutoRespawn = tanker["autorespawn"].get<std::string>() == "true",
					.AirbossRecovery = tanker["airboss_recovery"].get<std::string>() == "true",
					.MaxMissionDuration = tanker["missionmaxduration"].get<int>(),
					.Altitude = tanker["altitude"].get<int>(),
					.Speed = tanker["speed"].get<int>(),
					.FuelWarningLevel = tanker["fuelwarninglevel"].get<int>(),
					.Modex = tanker["modex"].get<int>(),
					.TacanChannel = tanker["tacan"]["channel"].get<int>(),
					.RacetrackFront = tanker["racetrack"]["front"].get<int>(),
					.RacetrackBack = tanker["racetrack"]["back"].get<int>(),
					.CallsignNb = tanker["callsign"]["number"].get<int>()
				};
				m_tankers.push_back(tanker_object);
			}
		}
		else if (name.find("settings-carriers") != std::string::npos)
		{
		}
		else if (name.find("settings-beacons") != std::string::npos)
		{
		}
		else if (name.find("settings-awacs") != std::string::npos)
		{
		}
		else if (name.find("settings-atis") != std::string::npos)
		{
		}
	}
}
