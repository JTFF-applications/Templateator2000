#include "Utilities/Log.h"
#include "Utilities/Moose.h"

// ReSharper disable StringLiteralTypo
const std::map<const std::string, const std::map<const std::string, const std::string>> Moose::Airbases =
{
	{
		"Caucasus",
		{
			{"Gelendzhik", "AIRBASE.Caucasus.Gelendzhik"},
			{"Krasnodar Pashkovsky", "AIRBASE.Caucasus.Krasnodar_Pashkovsky"},
			{"Sukhumi Babushara", "AIRBASE.Caucasus.Sukhumi_Babushara"},
			{"Gudauta", "AIRBASE.Caucasus.Gudauta"},
			{"Batumi", "AIRBASE.Caucasus.Batumi"},
			{"Senaki Kolkhi", "AIRBASE.Caucasus.Senaki_Kolkhi"},
			{"Kobuleti", "AIRBASE.Caucasus.Kobuleti"},
			{"Kutaisi", "AIRBASE.Caucasus.Kutaisi"},
			{"Tbilisi Lochini", "AIRBASE.Caucasus.Tbilisi_Lochini"},
			{"Soganlug", "AIRBASE.Caucasus.Soganlug"},
			{"Vaziani", "AIRBASE.Caucasus.Vaziani"},
			{"Anapa Vityazevo", "AIRBASE.Caucasus.Anapa_Vityazevo"},
			{"Krasnodar Center", "AIRBASE.Caucasus.Krasnodar_Center"},
			{"Novorossiysk", "AIRBASE.Caucasus.Novorossiysk"},
			{"Krymsk", "AIRBASE.Caucasus.Krymsk"},
			{"Maykop Khanskaya", "AIRBASE.Caucasus.Maykop_Khanskaya"},
			{"Sochi Adler", "AIRBASE.Caucasus.Sochi_Adler"},
			{"Mineralnye Vody", "AIRBASE.Caucasus.Mineralnye_Vody"},
			{"Nalchik", "AIRBASE.Caucasus.Nalchik"},
			{"Mozdok", "AIRBASE.Caucasus.Mozdok"},
			{"Beslan", "AIRBASE.Caucasus.Beslan"},
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
			{"Abu Dhabi", "AIRBASE.PersianGulf.Abu_Dhabi_International_Airport"},
			{"Abu Musa", "AIRBASE.PersianGulf.Abu_Musa_Island_Airport"},
			{"Al-Bateen", "AIRBASE.PersianGulf.Al-Bateen_Airport"},
			{"Al Ain", "AIRBASE.PersianGulf.Al_Ain_International_Airport"},
			{"Al Dhafra", "AIRBASE.PersianGulf.Al_Dhafra_AB"},
			{"Al Maktoum", "AIRBASE.PersianGulf.Al_Maktoum_Intl"},
			{"Al Minhad", "AIRBASE.PersianGulf.Al_Minhad_AB"},
			{"Bandar e Jask", "AIRBASE.PersianGulf.Bandar_e_Jask_airfield"},
			{"Bandar Abbas", "AIRBASE.PersianGulf.Bandar_Abbas_Intl"},
			{"Bandar Lengeh", "AIRBASE.PersianGulf.Bandar_Lengeh"},
			{"Dubai", "AIRBASE.PersianGulf.Dubai_Intl"},
			{"Fujairah", "AIRBASE.PersianGulf.Fujairah_Intl"},
			{"Havadarya", "AIRBASE.PersianGulf.Havadarya"},
			{"Jiroft", "AIRBASE.PersianGulf.Jiroft_Airport"},
			{"Kerman", "AIRBASE.PersianGulf.Kerman_Airport"},
			{"Khasab", "AIRBASE.PersianGulf.Khasab"},
			{"Kish", "AIRBASE.PersianGulf.Kish_International_Airport"},
			{"Lar", "AIRBASE.PersianGulf.Lar_Airbase"},
			{"Lavan Island", "AIRBASE.PersianGulf.Lavan_Island_Airport"},
			{"Liwa", "AIRBASE.PersianGulf.Liwa_Airbase"},
			{"Qeshm Island", "AIRBASE.PersianGulf.Qeshm_Island"},
			{"Ras Al Khaimah", "AIRBASE.PersianGulf.Ras_Al_Khaimah_International_Airport"},
			{"Sas Al Nakheel", "AIRBASE.PersianGulf.Sas_Al_Nakheel_Airport"},
			{"Sharjah", "AIRBASE.PersianGulf.Sharjah_Intl"},
			{"Shiraz", "AIRBASE.PersianGulf.Shiraz_International_Airport"},
			{"Sir Abu Nuayr", "AIRBASE.PersianGulf.Sir_Abu_Nuayr"},
			{"Sirri", "AIRBASE.PersianGulf.Sirri_Island"},
			{"Tunb Island", "AIRBASE.PersianGulf.Tunb_Island_AFB"},
			{"Tunb Kochak", "AIRBASE.PersianGulf.Tunb_Kochak"},
		}
	},
	{
		"Syria",
		{
			{"Kuweires", "AIRBASE.Syria.Kuweires"},
			{"Marj Ruhayyil", "AIRBASE.Syria.Marj_Ruhayyil"},
			{"Kiryat Shmona", "AIRBASE.Syria.Kiryat_Shmona"},
			{"Marj as Sultan North", "AIRBASE.Syria.Marj_as_Sultan_North"},
			{"Eyn Shemer", "AIRBASE.Syria.Eyn_Shemer"},
			{"Incirlik", "AIRBASE.Syria.Incirlik"},
			{"Damascus", "AIRBASE.Syria.Damascus"},
			{"Bassel Al Assad", "AIRBASE.Syria.Bassel_Al_Assad"},
			{"Rosh Pina", "AIRBASE.Syria.Rosh_Pina"},
			{"Aleppo", "AIRBASE.Syria.Aleppo"},
			{"Al Qusayr", "AIRBASE.Syria.Al_Qusayr"},
			{"Wujah Al Hajar", "AIRBASE.Syria.Wujah_Al_Hajar"},
			{"Al Dumayr", "AIRBASE.Syria.Al_Dumayr"},
			{"Gazipasa", "AIRBASE.Syria.Gazipasa"},
			{"Hatay", "AIRBASE.Syria.Hatay"},
			{"Nicosia", "AIRBASE.Syria.Nicosia"},
			{"Pinarbashi", "AIRBASE.Syria.Pinarbashi"},
			{"Paphos", "AIRBASE.Syria.Paphos"},
			{"Kingsfield", "AIRBASE.Syria.Kingsfield"},
			{"Thalah", "AIRBASE.Syria.Thalah"},
			{"Haifa", "AIRBASE.Syria.Haifa"},
			{"Khalkhalah", "AIRBASE.Syria.Khalkhalah"},
			{"Megiddo", "AIRBASE.Syria.Megiddo"},
			{"Lakatamia", "AIRBASE.Syria.Lakatamia"},
			{"Rayak", "AIRBASE.Syria.Rayak"},
			{"Larnaca", "AIRBASE.Syria.Larnaca"},
			{"Mezzeh", "AIRBASE.Syria.Mezzeh"},
			{"Gecitkale", "AIRBASE.Syria.Gecitkale"},
			{"Akrotiri", "AIRBASE.Syria.Akrotiri"},
			{"Naqoura", "AIRBASE.Syria.Naqoura"},
			{"Gaziantep", "AIRBASE.Syria.Gaziantep"},
			{"Sayqal", "AIRBASE.Syria.Sayqal"},
			{"Tiyas", "AIRBASE.Syria.Tiyas"},
			{"Shayrat", "AIRBASE.Syria.Shayrat"},
			{"Taftanaz", "AIRBASE.Syria.Taftanaz"},
			{"H4", "AIRBASE.Syria.H4"},
			{"King Hussein Air College", "AIRBASE.Syria.King_Hussein_Air_College"},
			{"Rene Mouawad", "AIRBASE.Syria.Rene_Mouawad"},
			{"Jirah", "AIRBASE.Syria.Jirah"},
			{"Ramat David", "AIRBASE.Syria.Ramat_David"},
			{"Qabr as Sitt", "AIRBASE.Syria.Qabr_as_Sitt"},
			{"Minakh", "AIRBASE.Syria.Minakh"},
			{"Adana Sakirpasa", "AIRBASE.Syria.Adana_Sakirpasa"},
			{"Palmyra", "AIRBASE.Syria.Palmyra"},
			{"Hama", "AIRBASE.Syria.Hama"},
			{"Ercan", "AIRBASE.Syria.Ercan"},
			{"Marj as Sultan South", "AIRBASE.Syria.Marj_as_Sultan_South"},
			{"Tabqa", "AIRBASE.Syria.Tabqa"},
			{"Beirut Rafic Hariri", "AIRBASE.Syria.Beirut_Rafic_Hariri"},
			{"An Nasiriyah", "AIRBASE.Syria.An_Nasiriyah"},
			{"Abu al Duhur", "AIRBASE.Syria.Abu_al_Duhur"},
			{"At Tanf", "AIRBASE.Syria.At_Tanf"},
			{"H3", "AIRBASE.Syria.H3"},
			{"H3 Northwest", "AIRBASE.Syria.H3_Northwest"},
			{"H3 Southwest", "AIRBASE.Syria.H3_Southwest"},
			{"Kharab Ishk", "AIRBASE.Syria.Kharab_Ishk"},
			{"Raj al Issa East", "AIRBASE.Syria.Raj_al_Issa_East"},
			{"Raj al Issa West", "AIRBASE.Syria.Raj_al_Issa_West"},
			{"Ruwayshid", "AIRBASE.Syria.Ruwayshid"},
			{"Sanliurfa", "AIRBASE.Syria.Sanliurfa"},
			{"Tal Siman", "AIRBASE.Syria.Tal_Siman"},
			{"Deir ez Zor", "AIRBASE.Syria.Deir_ez_Zor"},
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
// ReSharper restore StringLiteralTypo

#define GET_MAP_FIRST_ROW(MAP) 	QStringList res;\
								for (const auto& data : (MAP)) \
									res.append(data.first.c_str()); \
								return res;

const QStringList Moose::GetQtAirbases()
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

const QStringList Moose::GetQtParkings()
{
	GET_MAP_FIRST_ROW(Parkings)
}

const QStringList Moose::GetQtAwacsCallsigns()
{
	GET_MAP_FIRST_ROW(AwacsCallsigns)
}

const QStringList Moose::GetQtTankerCallsigns()
{
	GET_MAP_FIRST_ROW(TankerCallsigns)
}

const std::string Moose::GetMooseAirbaseFromName(const std::string& airport_name)
{
	for (const auto& map : Airbases)
		for (const auto& pair : map.second)
			if (pair.first == airport_name)
				return pair.second;
	LOG_ERROR("Airport {} not found !", airport_name);
	throw std::exception("Airport not found !");
}

const std::string Moose::GetMooseCallsignFromName(const std::string& callsign_name)
{
	const auto tanker = std::ranges::find_if(TankerCallsigns,
	                                         [&](const std::pair<const std::string, const std::string>& pair)
	                                         {
		                                         return pair.first == callsign_name;
	                                         });
	const auto awacs = std::ranges::find_if(AwacsCallsigns,
	                                        [&](const std::pair<const std::string, const std::string>& pair)
	                                        {
		                                        return pair.first == callsign_name;
	                                        });

	if (tanker != TankerCallsigns.end())
		return tanker->second;
	if (awacs != AwacsCallsigns.end())
		return awacs->second;
	LOG_ERROR("Callsign {} not found !", callsign_name);
	throw std::exception("Callsign not found !");
}

const std::string Moose::GetMooseParkingFromName(const std::string& parking_size_name)
{
	try
	{
		return Parkings.at(parking_size_name);
	} catch ([[maybe_unused]] const std::exception& except)
	{
		LOG_ERROR("Parking {} not found !", parking_size_name);
		throw std::exception("Parking not found !");
	}
}

const std::string Moose::GetNameFromMooseAirbase(const std::string& airport)
{
	for (const auto& map : Airbases)
		for (const auto& pair : map.second)
			if (pair.second == airport)
				return pair.first;
	LOG_ERROR("Airport {} not found !", airport);
	throw std::exception("Airport not found !");
}

const std::string Moose::GetNameFromMooseCallsign(const std::string& callsign)
{
	const auto tanker = std::ranges::find_if(TankerCallsigns,
	                                         [&](const std::pair<const std::string, const std::string>& pair)
	                                         {
		                                         return pair.second == callsign;
	                                         });
	const auto awacs = std::ranges::find_if(AwacsCallsigns,
	                                        [&](const std::pair<const std::string, const std::string>& pair)
	                                        {
		                                        return pair.second == callsign;
	                                        });

	if (tanker != TankerCallsigns.end())
		return tanker->first;
	if (awacs != AwacsCallsigns.end())
		return awacs->first;
	LOG_ERROR("Callsign {} not found !", callsign);
	throw std::exception("Callsign not found !");
}

const std::string Moose::GetNameFromMooseParking(const std::string& parking_size)
{
	for (const auto& pair : Parkings)
		if (pair.second == parking_size)
			return pair.first;
	LOG_ERROR("Parking {} not found !", parking_size);
	throw std::exception("Parking not found !");
}
