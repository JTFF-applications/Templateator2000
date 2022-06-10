#include "Mission.h"

Mission::Mission()
	: m_initialized(false)
{
}

Mission::Mission(const std::filesystem::path& path)
	: m_initialized(true), m_path(path), m_dcsMission(path), m_scripts(path)
{
}

void Mission::Init(const std::filesystem::path& path)
{
	if (m_initialized)
		return;

	m_initialized = true;
	m_path = path;

	m_dcsMission.Init(m_path);
	m_scripts.Init(m_path);
}

const std::vector<Group> Mission::GetMissionGroups()
{
	std::vector<Group> res;
	res.insert(res.end(), m_dcsMission.GetHelicopters().begin(), m_dcsMission.GetHelicopters().end());
	res.insert(res.end(), m_dcsMission.GetPlanes().begin(), m_dcsMission.GetPlanes().end());
	res.insert(res.end(), m_dcsMission.GetShips().begin(), m_dcsMission.GetShips().end());
	res.insert(res.end(), m_dcsMission.GetStatics().begin(), m_dcsMission.GetStatics().end());
	res.insert(res.end(), m_dcsMission.GetVehicules().begin(), m_dcsMission.GetVehicules().end());
	return res;
}

#pragma region Tankers
const Tanker& Mission::GetTanker(const std::string& label) const
{
	return *std::find_if(m_scripts.m_tankers.begin(), m_scripts.m_tankers.end(), [&](const Tanker& tk) {
		return label == TANKER_PRESENTATION_STRING(tk);
		});
}

void Mission::AddTanker(const Tanker& tanker)
{
	m_scripts.m_tankers.push_back(tanker);
}

void Mission::ModifyTanker(const Tanker& tanker)
{
	std::replace_if(m_scripts.m_tankers.begin(), m_scripts.m_tankers.end(), [&](const Tanker& tk) {
		return TANKER_PRESENTATION_STRING(tanker) == TANKER_PRESENTATION_STRING(tk);
		}, tanker);
}

void Mission::DeleteTanker(const std::string& label)
{
	std::remove_if(m_scripts.m_tankers.begin(), m_scripts.m_tankers.end(), [&](const Tanker& tk) {
		return label == TANKER_PRESENTATION_STRING(tk);
		});
}
#pragma endregion