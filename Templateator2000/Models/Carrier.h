#pragma once

#include <nlohmann/json.hpp>

#include "Utilities/Coalition.h"
namespace json = nlohmann;

#include "Models/Models.h"

#define CARRIER_PRESENTATION_STRING(carrier) std::format("{} Tcn:{}{} Case{}", (carrier).Alias, (carrier).Tacan.Channel, (carrier).Tacan.Band, (carrier).RecoveryMode)

class Carrier final
{
public:
	static Carrier FromJson(const json::json& carrier);
	static json::json ToJson(const Carrier& carrier);

	friend bool operator==(const Carrier& lhs, const Carrier& rhs);
	friend bool operator!=(const Carrier& lhs, const Carrier& rhs) { return !(lhs == rhs); }

public:
	Coalition::Side Coalition;
	std::string Name, Alias, RecoveryTanker, AirbossDifficulty;
	std::string BaseFq, MarshallFq, LsoFq;
	std::string MarshallRelayUnit, LsoRelayUnit;
	std::string StatPath, TrapsheetsPath;
	bool MenuMarkZone, MenuSmokeZone, NiceGuy, HandleAi, InfinitePatrol, SingleCarrier;
	int ControlArea, RecoveryMode, MaxPatterns, MaxStacks;
	models::RecoveryOps RecoveryOps;
	models::MenuRecovery MenuRecovery;
	models::Tacan Tacan;
	models::Icls Icls;
};

inline Carrier Carrier::FromJson(const json::json& carrier)
{
	// ReSharper disable StringLiteralTypo
	const bool has_tanker = carrier.contains("recoverytanker");
	Carrier res = {
		.Coalition = Coalition::FromString(carrier["coalition"]),
		.Name = carrier["carriername"],
		.Alias = carrier["alias"],
		.RecoveryTanker = has_tanker ? carrier["recoverytanker"] : "",
		.AirbossDifficulty = carrier["difficulty"],
		.BaseFq = std::format("{:.3f}", carrier["freq"]["base"].get<float>()),
		.MarshallFq = std::format("{:.3f}", carrier["freq"]["marshall"].get<float>()),
		.LsoFq = std::format("{:.3f}", carrier["freq"]["lso"].get<float>()),
		.MarshallRelayUnit = carrier["releayunit"]["marshall"],
		.LsoRelayUnit = carrier["releayunit"]["lso"],
		.StatPath = carrier["operationsstatspath"],
		.TrapsheetsPath = carrier["operationstrapsheetpath"],
		.MenuMarkZone = carrier["enable_menumarkzones"],
		.MenuSmokeZone = carrier["enable_menusmokezones"],
		.NiceGuy = carrier["enable_niceguy"],
		.HandleAi = carrier["handleAI"],
		.InfinitePatrol = carrier["infintepatrol"],
		.SingleCarrier = carrier["singlecarrier"],
		.ControlArea = carrier["controlarea"],
		.RecoveryMode = carrier["recoverycase"],
		.MaxPatterns = carrier["maxpatterns"],
		.MaxStacks = carrier["maxstacks"],
		.RecoveryOps = models::RecoveryOps::FromJson(carrier["recoveryops"]),
		.MenuRecovery = models::MenuRecovery::FromJson(carrier["menurecovery"]),
		.Tacan = {
			.Channel = carrier["tacan"]["channel"],
			.Band = carrier["tacan"]["mode"],
			.Morse = carrier["tacan"]["morse"],
		},
		.Icls = {
			.Channel = carrier["icls"]["channel"],
			.Morse = carrier["icls"]["morse"]
		}
	};
	return res;
	// ReSharper restore StringLiteralTypo
}

inline json::json Carrier::ToJson(const Carrier& carrier)
{
	// ReSharper disable StringLiteralTypo
	json::json tacan = {};
	tacan["channel"] = carrier.Tacan.Channel;
	tacan["mode"] = carrier.Tacan.Band;
	tacan["morse"] = carrier.Tacan.Morse;

	json::json freq = {};
	freq["base"] = std::stod(carrier.BaseFq);
	freq["marshall"] = std::stod(carrier.MarshallFq);
	freq["lso"] = std::stod(carrier.LsoFq);

	json::json icls = {};
	icls["channel"] = carrier.Icls.Channel;
	icls["morse"] = carrier.Icls.Morse;

	json::json relay_unit = {};
	relay_unit["marshall"] = carrier.MarshallRelayUnit;
	relay_unit["lso"] = carrier.LsoRelayUnit;

	json::json recovery_ops = {};
	if (carrier.RecoveryOps.Type == models::RecoveryOps::Cyclic)
	{
		json::json cyclic = {};
		cyclic["event_duration_minutes"] = carrier.RecoveryOps.EventDuration;
		cyclic["event_ia_reserved_minutes"] = carrier.RecoveryOps.EventIaDuration;

		recovery_ops["mode"] = "cyclic";
		recovery_ops["cyclic"] = cyclic;
	}
	else
	{
		json::json recoveries = {};
		for (const auto& recovery_model : carrier.RecoveryOps.Recoveries)
		{
			json::json recovery = {};
			recovery["recovery_start_minutes"] = recovery_model.Start;
			recovery["recovery_duration_minutes"] = recovery_model.Duration;
			recovery["recovery_case"] = recovery_model.RecoveryMode;
			recoveries += recovery;
		}

		json::json alpha = {};
		alpha["recoveries"] = recoveries;

		recovery_ops["mode"] = "alpha";
		recovery_ops["alpha"] = alpha;
	}

	json::json menu_recovery = {};
	menu_recovery["enable"] = true;
	menu_recovery["duration"] = carrier.MenuRecovery.Duration;
	menu_recovery["windondeck"] = carrier.MenuRecovery.WindOnDeck;
	menu_recovery["offset"] = carrier.MenuRecovery.Offset;
	menu_recovery["uturn"] = carrier.MenuRecovery.UTurn;

	json::json res = {};
	res["enable"] = true;
	res["carriername"] = carrier.Name;
	res["alias"] = carrier.Alias;
	res["coalition"] = Coalition::ToString(carrier.Coalition);
	res["enable_menumarkzones"] = carrier.MenuMarkZone;
	res["enable_menusmokezones"] = carrier.MenuSmokeZone;
	res["enable_niceguy"] = carrier.NiceGuy;
	res["handleAI"] = carrier.HandleAi;
	res["recoverytanker"] = carrier.RecoveryTanker;
	res["recoveryops"] = recovery_ops;
	res["tacan"] = tacan;
	res["icls"] = icls;
	res["freq"] = freq;
	res["infintepatrol"] = carrier.InfinitePatrol;
	res["controlarea"] = carrier.ControlArea;
	res["recoverycase"] = carrier.RecoveryMode;
	res["maxpatterns"] = carrier.MaxPatterns;
	res["maxstacks"] = carrier.MaxStacks;
	res["difficulty"] = carrier.AirbossDifficulty;
	res["menurecovery"] = menu_recovery;
	res["releayunit"] = relay_unit;
	res["singlecarrier"] = carrier.SingleCarrier;
	res["operationsstatspath"] = carrier.StatPath;
	res["operationstrapsheetpath"] = carrier.TrapsheetsPath;

	return res;
	// ReSharper enable StringLiteralTypo
}

inline bool operator==(const Carrier& lhs, const Carrier& rhs)
{
	return lhs.Coalition == rhs.Coalition
	       && lhs.Name == rhs.Name
	       && lhs.Alias == rhs.Alias
	       && lhs.RecoveryTanker == rhs.RecoveryTanker
	       && lhs.AirbossDifficulty == rhs.AirbossDifficulty
	       && lhs.BaseFq == rhs.BaseFq
	       && lhs.MarshallFq == rhs.MarshallFq
	       && lhs.LsoFq == rhs.LsoFq
	       && lhs.MarshallRelayUnit == rhs.MarshallRelayUnit
	       && lhs.LsoRelayUnit == rhs.LsoRelayUnit
	       && lhs.StatPath == rhs.StatPath
	       && lhs.TrapsheetsPath == rhs.TrapsheetsPath
	       && lhs.MenuMarkZone == rhs.MenuMarkZone
	       && lhs.MenuSmokeZone == rhs.MenuSmokeZone
	       && lhs.NiceGuy == rhs.NiceGuy
	       && lhs.HandleAi == rhs.HandleAi
	       && lhs.InfinitePatrol == rhs.InfinitePatrol
	       && lhs.SingleCarrier == rhs.SingleCarrier
	       && lhs.ControlArea == rhs.ControlArea
	       && lhs.RecoveryMode == rhs.RecoveryMode
	       && lhs.MaxPatterns == rhs.MaxPatterns
	       && lhs.MaxStacks == rhs.MaxStacks
	       && lhs.RecoveryOps == rhs.RecoveryOps
	       && lhs.MenuRecovery == rhs.MenuRecovery
	       && lhs.Tacan == rhs.Tacan
	       && lhs.Icls == rhs.Icls;
}
