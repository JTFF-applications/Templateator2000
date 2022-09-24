#pragma once

#include <string>

#include <nlohmann/json.hpp>
namespace json = nlohmann;

#include "Models/Models.h"
#include "Utilities/Moose.h"

#define ATIS_PRESENTATION_STRING(atis) std::format("{} Fq:{} Tcn:{}{}", (atis).AirportName, (atis).Radio.Frequency, (atis).Tacan.Channel, (atis).Tacan.Band)

class Atis final
{
public:
	static Atis FromJson(const json::json& atis);
	static json::json ToJson(const Atis& atis);

	friend bool operator==(const Atis& lhs, const Atis& rhs);
	friend bool operator!=(const Atis& lhs, const Atis& rhs);

public:
	std::string AirportName;
	std::string ActiveRunwayNumber, ActiveRunwaySide;
	models::Radio Radio;
	models::Tacan Tacan;
	models::Ils Ils;
	std::string SrsPath;
};

inline Atis Atis::FromJson(const json::json& atis)
{
	const bool has_runway = atis.contains("active");
	const bool has_radio = atis["radio"].contains("tower");
	const bool has_ils = atis.contains("ils");
	const bool has_tacan = atis.contains("tacan");

	std::vector<std::string> tower_frequencies;
	if (has_radio)
		std::ranges::transform(atis["radio"]["tower"],
		                       std::back_inserter(tower_frequencies),
		                       [&](const double& nb)
		                       {
			                       return std::format("{:.3f}", nb);
		                       });

	Atis res = {
		.AirportName = atis["airfield"],
		.ActiveRunwayNumber = has_runway ? atis["active"]["number"] : "",
		.ActiveRunwaySide = has_runway ? atis["active"]["side"] : "",
		.Radio = {
			.Frequency = std::format("{:.3f}", atis["radio"]["freq"].get<float>()),
			.Modulation = Moose::GetModulationFromNumber(atis["radio"]["modulation"].get<int>()),
			.RelayUnit = atis["radio"]["relayunit"],
			.TowerFrequencies = tower_frequencies,
			.Power = atis["radio"]["power"]
		},
		.Tacan = {
			.Channel = has_tacan ? atis["tacan"]["channel"].get<int>() : 0,
			.Band = has_tacan ? atis["tacan"]["band"] : "",
			.Morse = has_tacan ? atis["tacan"]["morse"] : ""
		},
		.Ils = {
			.Frequency = has_ils ? std::format("{:.3f}", atis["ils"]["freq"].get<float>()) : "",
			.Runway = has_ils ? atis["ils"]["runway"] : ""
		},
		.SrsPath = atis["srs"]["path"],
	};
	return res;
}

inline json::json Atis::ToJson(const Atis& atis)
{
	const bool has_runway = !atis.ActiveRunwayNumber.empty();
	const bool has_ils = !atis.Ils.Frequency.empty();
	const bool has_tacan = !atis.Tacan.Morse.empty();

	json::json active_runway = {};
	active_runway["number"] = atis.ActiveRunwayNumber;
	active_runway["side"] = atis.ActiveRunwaySide;

	json::json srs = {};
	srs["path"] = atis.SrsPath;

	json::json res = {};
	res["enable"] = true;
	res["airfield"] = atis.AirportName;
	res["radio"] = models::Radio::ToJson(atis.Radio);
	if (has_runway)
		res["active"] = active_runway;
	if (has_tacan)
		res["tacan"] = models::Tacan::ToJson(atis.Tacan);
	if (has_ils)
		res["ils"] = models::Ils::ToJson(atis.Ils);
	res["srs"] = srs;

	return res;
}

inline bool operator==(const Atis& lhs, const Atis& rhs)
{
	return lhs.AirportName == rhs.AirportName
	       && lhs.ActiveRunwayNumber == rhs.ActiveRunwayNumber
	       && lhs.ActiveRunwaySide == rhs.ActiveRunwaySide
	       && lhs.Radio == rhs.Radio
	       && lhs.Tacan == rhs.Tacan
	       && lhs.Ils == rhs.Ils
	       && lhs.SrsPath == rhs.SrsPath;
}
