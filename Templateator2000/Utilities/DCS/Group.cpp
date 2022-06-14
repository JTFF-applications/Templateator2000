#include "Utilities/DCS/Group.h"

Group::Group(const std::string& name, const std::vector<Unit>& units)
	: m_name(name), m_units(units)
{
}
