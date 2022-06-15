#pragma once

#include <QList>

#include <map>
#include <string>

class Moose
{
public:
	static const std::map<const std::string, const std::map<const std::string, const std::string>> Airbases;
	static const std::map<const std::string, const std::string> Parkings;
	static const std::map<const std::string, const std::string> AwacsCallsigns;
	static const std::map<const std::string, const std::string> TankerCallsigns;

	static const QStringList GetQtAirbases();
	static const QStringList GetQtParkings();
	static const QStringList GetQtAwacsCallsigns();
	static const QStringList GetQtTankerCallsigns();

	static const std::string GetMooseAirbaseFromName(const std::string& airport_name);
	static const std::string GetMooseCallsignFromName(const std::string& callsign_name);
	static const std::string GetMooseParkingFromName(const std::string& parking_size_name);

	static const std::string GetNameFromMooseAirbase(const std::string& airport);
	static const std::string GetNameFromMooseCallsign(const std::string& callsign);
	static const std::string GetNameFromMooseParking(const std::string& parking_size);
};
