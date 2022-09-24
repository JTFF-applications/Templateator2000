#pragma once

#include <nlohmann/json.hpp>
namespace json = nlohmann;

#include "Models/Models.h"

#define BEACON_PRESENTATION_STRING(beacon) std::format("{} Tcn:{}{} {}", (beacon).Name, (beacon).Tacan.Channel, (beacon).Tacan.Band, (beacon).Tacan.Morse)

class Beacon
{
public:
	static Beacon FromJson(const json::json& beacon);
	static json::json ToJson(const Beacon& beacon);

	friend bool operator==(const Beacon& lhs, const Beacon& rhs);
	friend bool operator!=(const Beacon& lhs, const Beacon& rhs) { return !(lhs == rhs); }

public:
	std::string Name, UnitName;
	models::Tacan Tacan;
};

inline Beacon Beacon::FromJson(const json::json& beacon)
{
	// ReSharper disable StringLiteralTypo
	Beacon res = {
		.Name = beacon["name"],
		.UnitName = beacon["unitName"],
		.Tacan = {
			.Channel = beacon["tacan"]["channel"],
			.Band = beacon["tacan"]["band"],
			.Morse = beacon["tacan"]["morse"]
		}
	};
	return res;
	// ReSharper restore StringLiteralTypo
}

inline json::json Beacon::ToJson(const Beacon& beacon)
{
	// ReSharper disable StringLiteralTypo
	json::json res;
	res["enable"] = true;
	res["name"] = beacon.Name;
	res["unitName"] = beacon.UnitName;
	res["tacan"] = models::Tacan::ToJson(beacon.Tacan);
	return res;
	// ReSharper restore StringLiteralTypo
}

inline bool operator==(const Beacon& lhs, const Beacon& rhs)
{
	return lhs.Name == rhs.Name
	       && lhs.UnitName == rhs.UnitName
	       && lhs.Tacan == rhs.Tacan;
}
