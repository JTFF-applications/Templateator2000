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
	std::vector<std::string> tower_frequencies;
	std::ranges::transform(atis["radio"]["tower"],
	                       std::back_inserter(tower_frequencies),
	                       [&](const double& nb)
	                       {
		                       return std::format("{:.3f}", nb);
	                       });
	const bool has_ils = atis.contains("ils");
	Atis res = {
		.AirportName = atis["airfield"],
		.ActiveRunwayNumber = atis["active"]["number"],
		.ActiveRunwaySide = atis["active"]["side"],
		.Radio = {
			.Frequency = std::format("{:.3f}", atis["radio"]["freq"].get<float>()),
			.Modulation = Moose::GetModulationFromNumber(atis["radio"]["modulation"].get<int>()),
			.RelayUnit = atis["radio"]["relayunit"],
			.TowerFrequencies = tower_frequencies,
			.Power = atis["radio"]["power"]
		},
		.Tacan = {
			.Channel = atis["tacan"]["channel"],
			.Band = atis["tacan"]["band"]
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
	json::json active_runway = {};
	active_runway["number"] = atis.ActiveRunwayNumber;
	active_runway["side"] = atis.ActiveRunwaySide;

	json::json srs = {};
	srs["path"] = atis.SrsPath;

	json::json res = {};
	res["enable"] = true;
	res["airfield"] = atis.AirportName;
	res["radio"] = models::Radio::ToJson(atis.Radio);
	res["active"] = active_runway;
	if (atis.Tacan.Channel != 0)
		res["tacan"] = models::Tacan::ToJson(atis.Tacan);
	if (!atis.Ils.Frequency.empty())
		res["ils"] = models::Ils::ToJson(atis.Ils);
	res["srs"] = srs;

	return res;
}
