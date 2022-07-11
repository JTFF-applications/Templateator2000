#pragma once

#include <QList>

#include <map>
#include <string>

class Moose
{
public:
	// Airbases (MOOSE.AIRBASE)
	static const std::map<const std::string, const std::map<const std::string, const std::string>> Airbases;
	static const QStringList GetQtAirbases();
	static const std::string GetMooseAirbaseFromName(const std::string& airport_name);
	static const std::string GetNameFromMooseAirbase(const std::string& moose_airbase);

	// Parkings (MOOSE.AIRBASE.TerminalType)
	static const QStringList GetQtParkings();
	static const std::string GetMooseTerminalFromNumber(const int& nb);
	static int GetNumberFromMooseTerminal(const std::string& terminal);

	// Callsigns (MOOSE.CALLSIGN)
	static const std::map<const std::string, const std::string> TankerCallsigns;
	static const QStringList GetQtTankerCallsigns();
	static const std::string GetMooseCallsignFromName(const std::string& callsign_name);
	static const std::string GetNameFromMooseCallsign(const std::string& callsign);
	static const std::string GetMooseCallsignFromNumber(const int& nb, const std::string& moose_table);
	static int GetMooseNumberFromCallsign(const std::string& aircraft_type, const std::string& callsign);

	// OTHERS
	static int GetNumberFromModulation(const std::string& modulation);
	static std::string GetModulationFromNumber(const int& number);
};
