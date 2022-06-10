#pragma once

#include <string>

#include "Utilities/DCS/Unit.h"
#include "Utilities/Coalitions.h"

#define TANKER_PRESENTATION_STRING(tanker) std::format("{}-{} Fq:{} Tcn:{}Y {}", tanker.Callsign.substr(tanker.Callsign.find_last_of('.') + 1), tanker.CallsignNb, tanker.Frequency, tanker.TacanChannel, tanker.TacanMorse)

class Tanker
{
public:
	enum class Type
	{
		Fixed, OnDemand
	};

public:
	Type Type;
	Coalition Coalition;
	std::string PatternUnit, DepartureBase, Callsign, Frequency, TacanMorse;
	bool AutoRespawn, AirbossRecovery;
	int MaxMissionDuration, Altitude, Speed, FuelWarningLevel, Modex, TacanChannel;
	int RacetrackFront, RacetrackBack, CallsignNb;
};
