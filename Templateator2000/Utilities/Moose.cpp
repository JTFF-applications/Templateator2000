#include "Moose.h"

const std::map<const std::string, const std::map<const std::string, const std::string>> Moose::Airbases =
{
	{
		"Caucasus",
		{
			{"", ""}
		}
	},
	{
		"Nevada",
		{
			{"Creech", "AIRBASE.Nevada.Creech_AFB"},
			{"Groom Lake", "AIRBASE.Nevada.Groom_Lake_AFB"},
			{"Mc Carran", "AIRBASE.Nevada.McCarran_International_Airport"},
			{"Nellis", "AIRBASE.Nevada.Nellis_AFB"},
			{"Beatty", "AIRBASE.Nevada.Beatty_Airport"},
			{"Boulder City", "AIRBASE.Nevada.Boulder_City_Airport"},
			{"Echo Bay", "AIRBASE.Nevada.Echo_Bay"},
			{"Henderson", "AIRBASE.Nevada.Henderson_Executive_Airport"},
			{"Jean", "AIRBASE.Nevada.Jean_Airport"},
			{"Laughlin", "AIRBASE.Nevada.Laughlin_Airport"},
			{"Lincoln County", "AIRBASE.Nevada.Lincoln_County"},
			{"Mesquite", "AIRBASE.Nevada.Mesquite"},
			{"Mina", "AIRBASE.Nevada.Mina_Airport"},
			{"North Las Vegas", "AIRBASE.Nevada.North_Las_Vegas"},
			{"Pahute Mesa", "AIRBASE.Nevada.Pahute_Mesa_Airstrip"},
			{"Tonopah", "AIRBASE.Nevada.Tonopah_Airport"},
			{"Tonopah Test Range", "AIRBASE.Nevada.Tonopah_Test_Range_Airfield"},
		}
	},
	{
		"Persian Gulf",
		{
			{"", ""}
		}
	},
	{
		"Syria",
		{
			{"", ""}
		}
	}
};

const std::map<const std::string, const std::string> Moose::Parkings =
{
	{"Fighter", "AIRBASE.TerminalType.FighterAircraft"},
	{"Helicopter Only", "AIRBASE.TerminalType.HelicopterOnly"},
	{"Helicopter Usable", "AIRBASE.TerminalType.HelicopterUsable"},
	{"Open Big", "AIRBASE.TerminalType.OpenBig"},
	{"Open Medium", "AIRBASE.TerminalType.OpenMed"},
	{"Open Medium or Big", "AIRBASE.TerminalType.OpenMedOrBig"},
	{"Runway", "AIRBASE.TerminalType.Runway"},
	{"Shelter (Caucasus Only)", "AIRBASE.TerminalType.Shelter"}
};

const std::map<const std::string, const std::string> Moose::AwacsCallsigns =
{
	{"Overlord", "CALLSIGN.AWACS.Overlord"},
	{"Magic", "CALLSIGN.AWACS.Magic"},
	{"Wizard", "CALLSIGN.AWACS.Wizard"},
	{"Focus", "CALLSIGN.AWACS.Focus"},
	{"Darkstar", "CALLSIGN.AWACS.Darkstar"}
};

const std::map<const std::string, const std::string> Moose::TankerCallsigns =
{
	{"Texaco", "CALLSIGN.Tanker.Texaco"},
	{"Arco", "CALLSIGN.Tanker.Arco"},
	{"Shell", "CALLSIGN.Tanker.Shell"},
};

#define GET_MAP_FIRST_ROW(MAP) 	QStringList res;\
								for (const auto& data : MAP) \
									res.append(data.first.c_str()); \
								return res;

const QStringList Moose::GetQTAirbases()
{
	QStringList res;
	for (const auto& map : Airbases)
	{
		res.append(map.first.c_str());
		res.append("----------");
		for (const auto& pair : map.second)
			res.push_back(pair.first.c_str());
	}
	return res;
}

const QStringList Moose::GetQTParkings()
{
	GET_MAP_FIRST_ROW(Parkings)
}

const QStringList Moose::GetQTAwacsCallsigns()
{
	GET_MAP_FIRST_ROW(AwacsCallsigns)
}

const QStringList Moose::GetQTTankerCallsigns()
{
	GET_MAP_FIRST_ROW(TankerCallsigns)
}
