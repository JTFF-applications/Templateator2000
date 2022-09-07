#pragma once

#include <string>

#include <nlohmann/json.hpp>
namespace json = nlohmann;

#include "Models/Models.h"
#include "Utilities/Coalition.h"
#include "Utilities/Moose.h"

#define TANKER_PRESENTATION_STRING(tanker) std::format("{} {}-{} Fq:{} Tcn:{}{} {}", ((tanker).Type == Tanker::Type::Fixed) ? "FIXED" : "ON DEMAND", (tanker).Callsign.Name.substr((tanker).Callsign.Name.find_last_of('.') + 1), (tanker).Callsign.Number, (tanker).Frequency, (tanker).Tacan.Channel, (tanker).Tacan.Band, (tanker).Tacan.Morse)

class Tanker final
{
public:
	enum class Type
	{
		Fixed, OnDemand
	};

public:
	static Tanker FromJson(const json::json& tanker, const Tanker::Type& type);
	static json::json ToJson(const Tanker& tanker, const Tanker::Type& type);

public:
	Type Type;
	Coalition::Side Coalition;

	// GENERAL
	bool AutoRespawn, AirbossRecovery;
	std::string DepartureBase, TerminalType, GroupName, EscortGroupName, Frequency;
	int MaxMissionDuration, FuelWarningLevel, Modex;

	models::Tacan Tacan;
	models::Callsign Callsign;

	// FIXED
	std::string PatternUnit;
	int Altitude, Speed;
	models::RaceTrack Racetrack;

	// ON DEMAND
	std::string Name;
	models::Orbit Orbit;
};

inline Tanker Tanker::FromJson(const json::json& tanker, const enum Tanker::Type& type)
{
	// ReSharper disable StringLiteralTypo
	const bool is_escorted = tanker.contains("escortgroupname");
	const bool is_default_tacan_band = !tanker["tacan"].contains("band");
	const bool is_fixed = type == Type::Fixed;
	const bool ondemand_has_infos = type == Type::OnDemand && tanker.contains("altitude");

	Tanker res = {
		.Type = type,
		.Coalition = Coalition::FromDcsCoalition(tanker["benefit_coalition"]),
		.AutoRespawn = tanker["autorespawn"],
		.AirbossRecovery = tanker["airboss_recovery"],
		.DepartureBase = tanker["baseUnit"],
		.TerminalType = Moose::GetMooseTerminalFromNumber(tanker["terminalType"]),
		.GroupName = tanker["groupName"],
		.EscortGroupName = is_escorted ? tanker["escortgroupname"] : "",
		.Frequency = std::format("{:.3f}", tanker["freq"].get<float>()),
		.MaxMissionDuration = tanker["missionmaxduration"],
		.FuelWarningLevel = tanker["fuelwarninglevel"],
		.Modex = tanker["modex"],
		.Tacan = {
			.Channel = tanker["tacan"]["channel"],
			.Band = is_default_tacan_band ? "Y" : tanker["tacan"]["band"],
			.Morse = tanker["tacan"]["morse"],
		},
		.Callsign = {
			.Name = Moose::GetMooseCallsignFromNumber(tanker["callsign"]["name"],"CALLSIGN.Tanker"),
			.Number = tanker["callsign"]["number"]
		},
		.PatternUnit = is_fixed ? tanker["patternUnit"] : "",
		.Altitude = is_fixed || ondemand_has_infos ? tanker["altitude"].get<int>() : 0,
		.Speed = is_fixed || ondemand_has_infos ? tanker["speed"].get<int>() : 0,
		.Racetrack = {
			.Front = is_fixed ? tanker["racetrack"]["front"].get<int>() : 0,
			.Back = is_fixed ? tanker["racetrack"]["back"].get<int>() : 0,
		},
		.Name = !is_fixed ? tanker["type"] : "",
		.Orbit = {
			.Heading = !is_fixed ? tanker["orbit"]["heading"].get<int>() : 0,
			.Length = !is_fixed ? tanker["orbit"]["length"].get<int>() : 0,
		},
	};
	return res;
	// ReSharper restore StringLiteralTypo
}

inline json::json Tanker::ToJson(const Tanker& tanker, const enum Type& type)
{
	// ReSharper disable StringLiteralTypo
	json::json result = {};
    result["enable"] = true;
	result["benefit_coalition"] = tanker.Coalition;
	result["autorespawn"] = tanker.AutoRespawn;
	result["airboss_recovery"] = tanker.AirbossRecovery;
	result["baseUnit"] = tanker.DepartureBase;
	result["terminalType"] = Moose::GetNumberFromMooseTerminal(tanker.TerminalType);
	result["groupName"] = tanker.GroupName;
	if(!tanker.EscortGroupName.empty())
		result["escortgroupname"] = tanker.EscortGroupName;
	result["freq"] = std::stod(tanker.Frequency);
	result["missionmaxduration"] = tanker.MaxMissionDuration;
	result["fuelwarninglevel"] = tanker.FuelWarningLevel;
    result["modex"] = tanker.Modex;
	result["tacan"] = models::Tacan::ToJson(tanker.Tacan);
    result["callsign"] = models::Callsign::ToJson(tanker.Callsign, "Tanker");

	if (tanker.Altitude != 0)
		result["altitude"] = tanker.Altitude;
	if (tanker.Speed != 0)
		result["speed"] = tanker.Speed;
	if(tanker.Type == Type::Fixed)
	{
		result["patternUnit"] = tanker.PatternUnit;
		result["racetrack"] = models::RaceTrack::ToJson(tanker.Racetrack);
	}
	else if (tanker.Type == Type::OnDemand)
	{
		result["type"] = tanker.Name;
		result["orbit"] = models::Orbit::ToJson(tanker.Orbit);
	}

    return result;
	// ReSharper restore StringLiteralTypo
}
