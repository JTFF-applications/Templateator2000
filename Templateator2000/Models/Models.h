#pragma once

#include "Utilities/Moose.h"

namespace models
{
	class Tacan final
	{
	public:
		static json::json ToJson(const Tacan& tacan)
		{
			json::json res = {};
			res["channel"] = tacan.Channel;
			res["band"] = tacan.Band;
			res["morse"] = tacan.Morse;
			return res;
		}

	public:
		int Channel;
		std::string Band, Morse;
	};

	class Ils final
	{
	public:
		static json::json ToJson(const Ils& ils)
		{
			json::json res = {};
			res["freq"] = std::stod(ils.Frequency);
			res["runway"] = ils.Runway;
			return res;
		}

	public:
		std::string Frequency;
		std::string Runway;
	};

	class Radio final
	{
	public:
		static json::json ToJson(const Radio& radio)
		{
			json::json res = {};
			res["freq"] = std::stod(radio.Frequency);
			res["power"] = radio.Power;
			res["modulation"] = Moose::GetNumberFromModulation(radio.Modulation);
			res["relayunit"] = radio.RelayUnit;

			std::vector<double> tower_frequencies;
			std::ranges::transform(radio.TowerFrequencies,
			                       std::back_inserter(tower_frequencies),
			                       [&](const std::string& frequency)
			                       {
				                       return std::stod(frequency);
			                       });
			res["tower"] = tower_frequencies;
			return res;
		}

	public:
		std::string Frequency, Modulation, RelayUnit;
		std::vector<std::string> TowerFrequencies;
		int Power;
	};

	class RaceTrack final
	{
	public:
		static json::json ToJson(const RaceTrack& racetrack)
		{
			json::json res = {};
			res["front"] = racetrack.Front;
			res["back"] = racetrack.Back;
			return res;
		}

	public:
		int Front, Back;
	};

	class Callsign final
	{
	public:
		static json::json ToJson(const Callsign& callsign)
		{
			json::json res = {};
			res["alias"] = callsign.Name;
			res["name"] = Moose::GetMooseNumberFromCallsign("Tanker", callsign.Name);
			res["number"] = callsign.Number;
			return res;
		}

	public:
		std::string Name;
		int Number;
	};
}
