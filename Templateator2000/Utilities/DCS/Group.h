#pragma once

#include <string>
#include <vector>

#include "Utilities/DCS/Unit.h"

class Group
{
public:
	explicit Group(const std::string& name, const std::vector<Unit>& units);

	[[nodiscard]] const std::string& Name() const { return m_name; }
	[[nodiscard]] const std::vector<Unit>& Units() const { return m_units; }

private:
	std::string m_name;
	std::vector<Unit> m_units;
};
