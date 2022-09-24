#pragma once

#include <format>

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

		friend bool operator==(const Tacan& lhs, const Tacan& rhs)
		{
			return lhs.Channel == rhs.Channel
			       && lhs.Band == rhs.Band
			       && lhs.Morse == rhs.Morse;
		}

		friend bool operator!=(const Tacan& lhs, const Tacan& rhs) { return !(lhs == rhs); }

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

		friend bool operator==(const Ils& lhs, const Ils& rhs)
		{
			return lhs.Frequency == rhs.Frequency
			       && lhs.Runway == rhs.Runway;
		}

		friend bool operator!=(const Ils& lhs, const Ils& rhs) { return !(lhs == rhs); }

	public:
		std::string Frequency;
		std::string Runway;
	};

	class Icls final
	{
	public:
		static json::json ToJson(const Icls& icls)
		{
			json::json res = {};
			res["channel"] = icls.Channel;
			res["morse"] = icls.Morse;
			return res;
		}

		friend bool operator==(const Icls& lhs, const Icls& rhs)
		{
			return lhs.Channel == rhs.Channel
			       && lhs.Morse == rhs.Morse;
		}

		friend bool operator!=(const Icls& lhs, const Icls& rhs) { return !(lhs == rhs); }

	public:
		int Channel;
		std::string Morse;
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

			if (!radio.TowerFrequencies.empty())
			{
				std::vector<double> tower_frequencies;
				std::ranges::transform(radio.TowerFrequencies,
				                       std::back_inserter(tower_frequencies),
				                       [&](const std::string& frequency)
				                       {
					                       return std::stod(frequency);
				                       });
				res["tower"] = tower_frequencies;
			}
			return res;
		}

		friend bool operator==(const Radio& lhs, const Radio& rhs)
		{
			return lhs.Frequency == rhs.Frequency
			       && lhs.Modulation == rhs.Modulation
			       && lhs.RelayUnit == rhs.RelayUnit
			       && lhs.TowerFrequencies == rhs.TowerFrequencies
			       && lhs.Power == rhs.Power;
		}

		friend bool operator!=(const Radio& lhs, const Radio& rhs) { return !(lhs == rhs); }

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

		friend bool operator==(const RaceTrack& lhs, const RaceTrack& rhs)
		{
			return lhs.Front == rhs.Front
			       && lhs.Back == rhs.Back;
		}

		friend bool operator!=(const RaceTrack& lhs, const RaceTrack& rhs) { return !(lhs == rhs); }

	public:
		int Front, Back;
	};

	class Orbit final
	{
	public:
		static json::json ToJson(const Orbit& orbit)
		{
			json::json res = {};
			res["heading"] = orbit.Heading;
			res["length"] = orbit.Length;
			return res;
		}

		friend bool operator==(const Orbit& lhs, const Orbit& rhs)
		{
			return lhs.Heading == rhs.Heading
			       && lhs.Length == rhs.Length;
		}

		friend bool operator!=(const Orbit& lhs, const Orbit& rhs) { return !(lhs == rhs); }

	public:
		int Heading, Length;
	};

	class Callsign final
	{
	public:
		static json::json ToJson(const Callsign& callsign, const std::string& aircraft_type)
		{
			json::json res = {};
			res["alias"] = std::format("{}-{}", callsign.Name, callsign.Number);
			res["name"] = Moose::GetMooseNumberFromCallsign(aircraft_type, callsign.Name);
			res["number"] = callsign.Number;
			return res;
		}

		friend bool operator==(const Callsign& lhs, const Callsign& rhs)
		{
			return lhs.Name == rhs.Name
			       && lhs.Number == rhs.Number;
		}

		friend bool operator!=(const Callsign& lhs, const Callsign& rhs) { return !(lhs == rhs); }

	public:
		std::string Name;
		int Number;
	};

	class RecoveryOps final
	{
	public:
		static RecoveryOps FromJson(const json::json& recovery_ops)
		{
			const bool is_cyclic = recovery_ops["mode"] == "cyclic";
			std::vector<AlphaStrikeRecovery> recoveries;
			if (!is_cyclic)
				std::ranges::transform(recovery_ops["recoveries"],
				                       std::back_inserter(recoveries),
				                       [&](const json::json& recovery)
				                       {
					                       const AlphaStrikeRecovery res = {
						                       .Start = recovery["recovery_start_minutes"],
						                       .Duration = recovery["recovery_duration_minutes"],
						                       .RecoveryMode = recovery["recovery_case"]
					                       };
					                       return res;
				                       });

			RecoveryOps res = {
				.Type = is_cyclic ? Cyclic : AlphaStrike,
				.EventDuration = is_cyclic ? recovery_ops["cyclic"]["event_duration_minutes"].get<int>() : 0,
				.EventIaDuration = is_cyclic ? recovery_ops["cyclic"]["event_ia_reserved_minutes"].get<int>() : 0,
				.Recoveries = recoveries
			};
			return res;
		}

		friend bool operator==(const RecoveryOps& lhs, const RecoveryOps& rhs)
		{
			return lhs.Type == rhs.Type
			       && lhs.EventDuration == rhs.EventDuration
			       && lhs.EventIaDuration == rhs.EventIaDuration
			       && lhs.Recoveries == rhs.Recoveries;
		}

		friend bool operator!=(const RecoveryOps& lhs, const RecoveryOps& rhs) { return !(lhs == rhs); }

	public:
		enum Type
		{
			Cyclic, AlphaStrike
		};

		struct AlphaStrikeRecovery
		{
			friend bool operator==(const AlphaStrikeRecovery& lhs, const AlphaStrikeRecovery& rhs)
			{
				return lhs.Start == rhs.Start
				       && lhs.Duration == rhs.Duration
				       && lhs.RecoveryMode == rhs.RecoveryMode;
			}

			friend bool operator!=(const AlphaStrikeRecovery& lhs, const AlphaStrikeRecovery& rhs)
			{
				return !(lhs == rhs);
			}

			int Start, Duration, RecoveryMode;
		};

	public:
		Type Type;
		int EventDuration, EventIaDuration;
		std::vector<AlphaStrikeRecovery> Recoveries;
	};

	class MenuRecovery final
	{
	public:
		static MenuRecovery FromJson(const json::json& json)
		{
			// ReSharper disable StringLiteralTypo
			const MenuRecovery res = {
				.Duration = json["duration"],
				.WindOnDeck = json["windondeck"],
				.Offset = json["offset"],
				.UTurn = json["uturn"]
			};
			return res;
			// ReSharper restore StringLiteralTypo
		}

		friend bool operator==(const MenuRecovery& lhs, const MenuRecovery& rhs)
		{
			return lhs.Duration == rhs.Duration
			       && lhs.WindOnDeck == rhs.WindOnDeck
			       && lhs.Offset == rhs.Offset
			       && lhs.UTurn == rhs.UTurn;
		}

		friend bool operator!=(const MenuRecovery& lhs, const MenuRecovery& rhs) { return !(lhs == rhs); }

	public:
		int Duration, WindOnDeck, Offset;
		bool UTurn;
	};
}
