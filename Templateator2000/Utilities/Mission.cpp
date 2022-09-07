#include "Utilities/Mission.h"

QStringList Mission::DataToGroupName(const std::map<const std::string, const std::vector<Group>>& mission_data)
{
	QStringList res;
	for (const auto& groups : mission_data)
		for (const auto& group : groups.second)
			res.push_back(group.Name().c_str());
	return res;
}

QStringList Mission::DataToUnitName(const std::map<const std::string, const std::vector<Group>>& mission_data)
{
	QStringList res;
	for (const auto& groups : mission_data)
		for (const auto& group : groups.second)
			for (const auto& unit : group.Units())
				res.push_back(unit.Name().c_str());
	return res;
}

Mission::Mission()
	: m_initialized(false), m_is_saved(true)
{
}

Mission::Mission(const std::filesystem::path& path, const std::filesystem::path& mission_path)
	: m_path(path), m_mission_path(mission_path), m_initialized(true),
	  m_is_saved(true), m_dcs_mission(path), m_scripts(path)
{
}

void Mission::Init(const std::filesystem::path& path, const std::filesystem::path& mission_path)
{
	m_path = path;
	m_mission_path = mission_path;
	m_is_saved = true;

	m_dcs_mission.Init(m_path);
	m_scripts.Init(m_path);

	m_initialized = true;
}

void Mission::Save()
{
	m_scripts.Save();
	m_is_saved = true;
	copy_file(m_path, m_mission_path, std::filesystem::copy_options::overwrite_existing);
}

void Mission::SaveAs(const std::filesystem::path new_path)
{
	m_scripts.Save();
	m_is_saved = true;
	copy_file(m_path, new_path, std::filesystem::copy_options::overwrite_existing);
}

void Mission::ManageScripts()
{
	m_scripts.Manage();
	m_is_saved = false;
}

void Mission::UpdateScripts()
{
	m_scripts.Update();
	m_is_saved = false;
}

const std::map<const std::string, const std::vector<Group>> Mission::GetMissionGroups() const
{
	std::map<const std::string, const std::vector<Group>> res;
	res.emplace("helicopters", m_dcs_mission.GetHelicopters());
	res.emplace("planes", m_dcs_mission.GetPlanes());
	res.emplace("ships", m_dcs_mission.GetShips());
	res.emplace("statics", m_dcs_mission.GetStatics());
	res.emplace("vehicles", m_dcs_mission.GetVehicles());
	return res;
}

#pragma region Tankers
const Tanker& Mission::GetTanker(const std::string& label) const
{
	return *std::ranges::find_if(m_scripts.m_tankers,
	                             [&](const Tanker& tk)
	                             {
		                             return label == TANKER_PRESENTATION_STRING(tk);
	                             });
}

void Mission::AddTanker(const Tanker& tanker)
{
	m_scripts.m_tankers.push_back(tanker);
	m_is_saved = false;
}

void Mission::ModifyTanker(const Tanker& old_tanker, const Tanker& new_tanker)
{
	std::ranges::replace_if(m_scripts.m_tankers,
	                        [&](const Tanker& tk)
	                        {
		                        return TANKER_PRESENTATION_STRING(old_tanker) == TANKER_PRESENTATION_STRING(tk);
	                        },
	                        new_tanker);
	m_is_saved = false;
}

void Mission::RemoveTanker(const std::string& label)
{
	m_scripts.m_tankers.erase(std::ranges::find_if(m_scripts.m_tankers,
	                                               [&](const Tanker& tk)
	                                               {
		                                               return label == TANKER_PRESENTATION_STRING(tk);
	                                               }));
	m_is_saved = false;
}
#pragma endregion

#pragma region Atis
const Atis& Mission::GetAtis(const std::string& label) const
{
	return *std::ranges::find_if(m_scripts.m_atis,
	                             [&](const Atis& atis)
	                             {
		                             return label == ATIS_PRESENTATION_STRING(atis);
	                             });
}

void Mission::AddAtis(const Atis& atis)
{
	m_scripts.m_atis.push_back(atis);
	m_is_saved = false;
}

void Mission::ModifyAtis(const Atis& old_atis, const Atis& new_atis)
{
	std::ranges::replace_if(m_scripts.m_atis,
	                        [&](const Atis& atis)
	                        {
		                        return ATIS_PRESENTATION_STRING(old_atis) == ATIS_PRESENTATION_STRING(atis);
	                        },
	                        new_atis);
	m_is_saved = false;
}

void Mission::RemoveAtis(const std::string& label)
{
	m_scripts.m_atis.erase(std::ranges::find_if(m_scripts.m_atis,
	                                            [&](const Atis& atis)
	                                            {
		                                            return label == ATIS_PRESENTATION_STRING(atis);
	                                            }));
	m_is_saved = false;
}
#pragma endregion

#pragma region Beacons
const Beacon& Mission::GetBeacon(const std::string& label) const
{
	return *std::ranges::find_if(m_scripts.m_beacons,
	                             [&](const Beacon& beacon)
	                             {
		                             return label == BEACON_PRESENTATION_STRING(beacon);
	                             });
}

void Mission::AddBeacon(const Beacon& beacon)
{
	m_scripts.m_beacons.push_back(beacon);
	m_is_saved = false;
}

void Mission::ModifyBeacon(const Beacon& old_beacon, const Beacon& new_beacon)
{
	std::ranges::replace_if(m_scripts.m_beacons,
	                        [&](const Beacon& beacon)
	                        {
		                        return BEACON_PRESENTATION_STRING(old_beacon) == BEACON_PRESENTATION_STRING(beacon);
	                        },
	                        new_beacon);
	m_is_saved = false;
}

void Mission::RemoveBeacon(const std::string& label)
{
	m_scripts.m_beacons.erase(std::ranges::find_if(m_scripts.m_beacons,
	                                               [&](const Beacon& beacon)
	                                               {
		                                               return label == BEACON_PRESENTATION_STRING(beacon);
	                                               }));
	m_is_saved = false;
}
#pragma endregion

#pragma region Carriers
const Carrier& Mission::GetCarrier(const std::string& label) const
{
	return *std::ranges::find_if(m_scripts.m_carriers,
	                             [&](const Carrier& carrier)
	                             {
		                             return label == CARRIER_PRESENTATION_STRING(carrier);
	                             });
}

void Mission::AddCarrier(const Carrier& carrier)
{
	m_scripts.m_carriers.push_back(carrier);
	m_is_saved = false;
}

void Mission::ModifyCarrier(const Carrier& old_carrier, const Carrier& new_carrier)
{
	std::ranges::replace_if(m_scripts.m_carriers,
	                        [&](const Carrier& carrier)
	                        {
		                        return CARRIER_PRESENTATION_STRING(old_carrier) == CARRIER_PRESENTATION_STRING(carrier);
	                        },
	                        new_carrier);
	m_is_saved = false;
}

void Mission::RemoveCarrier(const std::string& label)
{
	m_scripts.m_carriers.erase(std::ranges::find_if(m_scripts.m_carriers,
	                                                [&](const Carrier& carrier)
	                                                {
		                                                return label == CARRIER_PRESENTATION_STRING(carrier);
	                                                }));
	m_is_saved = false;
}
#pragma endregion

#pragma region Awacs
const Awacs& Mission::GetAwacs(const std::string& label) const
{
	return *std::ranges::find_if(m_scripts.m_awacs,
	                             [&](const Awacs& awacs)
	                             {
		                             return label == AWACS_PRESENTATION_STRING(awacs);
	                             });
}

void Mission::AddAwacs(const Awacs& awacs)
{
	m_scripts.m_awacs.push_back(awacs);
	m_is_saved = false;
}

void Mission::ModifyAwacs(const Awacs& old_awacs, const Awacs& new_awacs)
{
	std::ranges::replace_if(m_scripts.m_awacs,
	                        [&](const Awacs& awacs)
	                        {
		                        return AWACS_PRESENTATION_STRING(old_awacs) == AWACS_PRESENTATION_STRING(awacs);
	                        },
	                        new_awacs);
	m_is_saved = false;
}

void Mission::RemoveAwacs(const std::string& label)
{
	m_scripts.m_awacs.erase(std::ranges::find_if(m_scripts.m_awacs,
	                                             [&](const Awacs& awacs)
	                                             {
		                                             return label == AWACS_PRESENTATION_STRING(awacs);
	                                             }));
	m_is_saved = false;
}
#pragma endregion
