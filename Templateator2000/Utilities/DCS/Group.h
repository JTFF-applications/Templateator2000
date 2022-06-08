#pragma once

#include <vector>
#include <string>

#include "Utilities/DCS/Unit.h"

class Group
{
public:
	Group(const std::string& name, const std::vector<Unit>& units);
	~Group() = default;

	const std::string& Name() const { return m_name; }

private:
	std::string m_name;
	std::vector<Unit> m_units;
};
