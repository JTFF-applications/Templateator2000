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

	static const QStringList GetQTAirbases();
	static const QStringList GetQTParkings();
	static const QStringList GetQTAwacsCallsigns();
	static const QStringList GetQTTankerCallsigns();
};
