#pragma once

#include <string>

#include <nlohmann/json.hpp>
namespace json = nlohmann;

#include "Models/Models.h"
#include "Utilities/Coalition.h"
#include "Utilities/Moose.h"

#define TANKER_PRESENTATION_STRING(tanker) std::format("{}-{} Fq:{} Tcn:{}{} {}", (tanker).Callsign.Name.substr((tanker).Callsign.Name.find_last_of('.') + 1), (tanker).Callsign.Number, (tanker).Frequency, (tanker).Tacan.Channel, (tanker).Tacan.Band, (tanker).Tacan.Morse)

class Tanker final
{
public:
	static Tanker FromJson(const json::json& tanker);
	static json::json ToJson(const Tanker& tanker);

public:
	enum class Type
	{
		Fixed, OnDemand
	};

public:
	Type Type;
	Coalition::Side Coalition;

	// GENERAL
	bool AutoRespawn, AirbossRecovery;
	std::string PatternUnit, DepartureBase, TerminalType, GroupName, EscortGroupName, Frequency;
	int MaxMissionDuration, Altitude, Speed, FuelWarningLevel, Modex;

	models::Tacan Tacan;
	models::RaceTrack Racetrack;
	models::Callsign Callsign;
};

inline Tanker Tanker::FromJson(const json::json& tanker)
{
	// ReSharper disable StringLiteralTypo
	const bool is_escorted = tanker.contains("escortgroupname");
	const bool is_default_tacan_band = !tanker["tacan"].contains("band");
	Tanker res = {
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
		.Tacan = {
			.Channel = tanker["tacan"]["channel"].get<int>(),
			.Band = is_default_tacan_band ? "Y" : tanker["tacan"]["band"].get<std::string>(),
			.Morse = tanker["tacan"]["morse"].get<std::string>(),
		},
		.Racetrack = {
			.Front = tanker["racetrack"]["front"].get<int>(),
			.Back = tanker["racetrack"]["back"].get<int>(),
		},
		.Callsign = {
			.Name = Moose::GetMooseCallsignFromNumber(tanker["callsign"]["name"].get<int>(),
													  "CALLSIGN.Tanker"),
			.Number = tanker["callsign"]["number"].get<int>()
		}
	};
	return res;
	// ReSharper restore StringLiteralTypo
}

inline json::json Tanker::ToJson(const Tanker& tanker)
{
	// ReSharper disable StringLiteralTypo
	json::json result = {};
    result["enable"] = true;
	result["benefit_coalition"] = tanker.Coalition;
	result["autorespawn"] = tanker.AutoRespawn;
	result["airboss_recovery"] = tanker.AirbossRecovery;
	result["patternUnit"] = tanker.PatternUnit;
	result["baseUnit"] = Moose::GetNameFromMooseAirbase(tanker.DepartureBase);
	result["terminalType"] = Moose::GetNumberFromMooseTerminal(tanker.TerminalType);
	result["groupName"] = tanker.GroupName;
	if(!tanker.EscortGroupName.empty())
		result["escortgroupname"] = tanker.EscortGroupName;
	result["freq"] = std::stod(tanker.Frequency);
	result["missionmaxduration"] = tanker.MaxMissionDuration;
	result["altitude"] = tanker.Altitude;
	result["speed"] = tanker.Speed;
	result["fuelwarninglevel"] = tanker.FuelWarningLevel;
    result["modex"] = tanker.Modex;
    result["racetrack"] = models::RaceTrack::ToJson(tanker.Racetrack);
	result["tacan"] = models::Tacan::ToJson(tanker.Tacan);
    result["callsign"] = models::Callsign::ToJson(tanker.Callsign, "Tanker");

    return result;
	// ReSharper restore StringLiteralTypo
}
