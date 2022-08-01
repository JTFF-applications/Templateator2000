#include "Utilities/DCS/Unit.h"

Unit::Unit(const std::string& type,
           const std::string& name,
           const std::string& skill,
           const Coalition::Side& coalition)
	: m_coalition(coalition), m_type(type), m_name(name), m_skill(skill)
{
}
