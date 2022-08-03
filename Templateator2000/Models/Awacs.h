#pragma once

#include <nlohmann/json.hpp>
namespace json = nlohmann;

#include "Models/Models.h"
#include "Utilities/Coalition.h"
#include "Utilities/Moose.h"

#define AWACS_PRESENTATION_STRING(awacs) std::format("{}-{} Fq:{} Tcn:{}{} {}", (awacs).Callsign.Name.substr((awacs).Callsign.Name.find_last_of('.') + 1), (awacs).Callsign.Number, (awacs).Frequency, (awacs).Tacan.Channel, (awacs).Tacan.Band, (awacs).Tacan.Morse)

class Awacs final
{
public:
	static Awacs FromJson(const json::json& awacs);
	static json::json ToJson(const Awacs& awacs);

public:
	Coalition::Side Coalition;

	// GENERAL
	bool AutoRespawn, AirbossRecovery;
	std::string PatternUnit, DepartureBase, TerminalType, GroupName, EscortGroupName, Frequency;
	int MaxMissionDuration, Altitude, Speed, FuelWarningLevel, Modex;

	models::Tacan Tacan;
	models::RaceTrack Racetrack;
	models::Callsign Callsign;
};

inline Awacs Awacs::FromJson(const json::json& awacs)
{
	// ReSharper disable StringLiteralTypo
	const bool is_escorted = awacs.contains("escortgroupname");
	const bool is_default_tacan_band = !awacs["tacan"].contains("band");
	Awacs res = {
		.Coalition = Coalition::FromDcsCoalition(awacs["benefit_coaleition"].get<int>()),
		.AutoRespawn = awacs["autorespawn"].get<bool>(),
		.AirbossRecovery = awacs["airboss_recovery"].get<bool>(),
		.PatternUnit = awacs["patternUnit"].get<std::string>(),
		.DepartureBase = awacs["baseUnit"].get<std::string>(),
		.TerminalType = Moose::GetMooseTerminalFromNumber(awacs["terminalType"].get<int>()),
		.GroupName = awacs["groupName"].get<std::string>(),
		.EscortGroupName = is_escorted ? awacs["escortgroupname"].get<std::string>() : "",
		.Frequency = std::format("{:.3f}", awacs["freq"].get<float>()),
		.MaxMissionDuration = awacs["missionmaxduration"].get<int>(),
		.Altitude = awacs["altitude"].get<int>(),
		.Speed = awacs["speed"].get<int>(),
		.FuelWarningLevel = awacs["fuelwarninglevel"].get<int>(),
		.Modex = awacs["modex"].get<int>(),
		.Tacan = {
			.Channel = awacs["tacan"]["channel"].get<int>(),
			.Band = is_default_tacan_band ? "Y" : awacs["tacan"]["band"].get<std::string>(),
			.Morse = awacs["tacan"]["morse"].get<std::string>(),
		},
		.Racetrack = {
			.Front = awacs["racetrack"]["front"].get<int>(),
			.Back = awacs["racetrack"]["back"].get<int>(),
		},
		.Callsign = {
			.Name = Moose::GetMooseCallsignFromNumber(awacs["callsign"]["name"].get<int>(),
			                                          "CALLSIGN.AWACS"),
			.Number = awacs["callsign"]["number"].get<int>()
		}
	};
	return res;
	// ReSharper restore StringLiteralTypo
}

inline json::json Awacs::ToJson(const Awacs& awacs)
{
	// ReSharper disable StringLiteralTypo
	json::json result = {};
	result["enable"] = true;
	result["benefit_coalition"] = awacs.Coalition;
	result["autorespawn"] = awacs.AutoRespawn;
	result["airboss_recovery"] = awacs.AirbossRecovery;
	result["patternUnit"] = awacs.PatternUnit;
	result["baseUnit"] = awacs.DepartureBase;
	result["terminalType"] = Moose::GetNumberFromMooseTerminal(awacs.TerminalType);
	result["groupName"] = awacs.GroupName;
	if (!awacs.EscortGroupName.empty())
		result["escortgroupname"] = awacs.EscortGroupName;
	result["freq"] = std::stod(awacs.Frequency);
	result["missionmaxduration"] = awacs.MaxMissionDuration;
	result["altitude"] = awacs.Altitude;
	result["speed"] = awacs.Speed;
	result["fuelwarninglevel"] = awacs.FuelWarningLevel;
	result["modex"] = awacs.Modex;
	result["racetrack"] = models::RaceTrack::ToJson(awacs.Racetrack);
	result["tacan"] = models::Tacan::ToJson(awacs.Tacan);
	result["callsign"] = models::Callsign::ToJson(awacs.Callsign, "AWACS");

	return result;
	// ReSharper restore StringLiteralTypo
}
