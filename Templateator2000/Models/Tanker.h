#pragma once

#include <nlohmann/json.hpp>
namespace json = nlohmann;

#include <string>

#include "Utilities/Coalitions.h"

#define TANKER_PRESENTATION_STRING(tanker) std::format("{}-{} Fq:{} Tcn:{}Y {}", (tanker).Callsign.substr((tanker).Callsign.find_last_of('.') + 1), (tanker).CallsignNb, (tanker).Frequency, (tanker).TacanChannel, (tanker).TacanMorse)

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
	Coalition Coalition;
	std::string PatternUnit, DepartureBase, ParkingSize, GroupName, EscortGroup, Callsign, Frequency, TacanMorse;
	bool AutoRespawn, AirbossRecovery;
	int MaxMissionDuration, Altitude, Speed, FuelWarningLevel, Modex, TacanChannel;
	int RacetrackFront, RacetrackBack, CallsignNb;
};

inline json::json Tanker::ToJson(const Tanker& tanker)
{
	throw std::logic_error("Not implemented");
}
