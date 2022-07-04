#pragma once

#include <nlohmann/json.hpp>

#include "Utilities/Moose.h"
namespace json = nlohmann;

#include <string>

#include "Utilities/Coalition.h"

#define TANKER_PRESENTATION_STRING(tanker) std::format("{}-{} Fq:{} Tcn:{}{} {}", (tanker).Callsign.substr((tanker).Callsign.find_last_of('.') + 1), (tanker).CallsignNb, (tanker).Frequency, (tanker).TacanChannel, (tanker).TacanBand, (tanker).TacanMorse)

class Tanker
{
public:
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

	// TACAN
	int TacanChannel;
	std::string TacanBand, TacanMorse;
	
	// RACETRACK
	int RacetrackFront, RacetrackBack;

	// CALLSIGN
	std::string Callsign;
	int CallsignNb;
};

inline json::json Tanker::ToJson(const Tanker& tanker)
{
	// ReSharper disable StringLiteralTypo
	json::json tacan = {};
	tacan["channel"] = tanker.TacanChannel;
	tacan["band"] = tanker.TacanBand;
	tacan["morse"] = tanker.TacanMorse;

	json::json racetrack = {};
	racetrack["front"] = tanker.RacetrackFront;
	racetrack["back"] = tanker.RacetrackBack;

	json::json callsign = {};
	callsign["alias"] = tanker.Callsign;
	callsign["name"] = Moose::GetMooseNumberFromCallsign("Tanker", tanker.Callsign);
	callsign["number"] = tanker.CallsignNb;

	json::json result = {};
    result["enable"] = true;
	result["benefit_coalition"] = tanker.Coalition;
	result["autorespawn"] = tanker.AutoRespawn;
	result["airboss_recovery"] = tanker.AirbossRecovery;
	result["patternUnit"] = tanker.PatternUnit;
	result["baseUnit"] = Moose::GetNameFromMooseAirbase(tanker.DepartureBase);
	result["terminalType"] = Moose::GetNumberFromMooseTerminal(tanker.TerminalType);
	result["groupName"] = tanker.GroupName;
	result["escortgroupname"] = tanker.EscortGroupName;
	result["freq"] = std::stod(tanker.Frequency);
	result["missionmaxduration"] = tanker.MaxMissionDuration;
	result["altitude"] = tanker.Altitude;
	result["speed"] = tanker.Speed;
	result["fuelwarninglevel"] = tanker.FuelWarningLevel;
    result["modex"] = tanker.Modex;
    result["racetrack"] = racetrack;
	result["tacan"] = tacan;
    result["callsign"] = callsign;

    return result;
	// ReSharper restore StringLiteralTypo
}
