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
	if (m_initialized)
		return;

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
