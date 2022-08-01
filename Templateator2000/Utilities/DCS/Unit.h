#pragma once

#include <string>

#include "Utilities/Coalition.h"

class Unit
{
public:
	explicit Unit(const std::string& type,
	              const std::string& name,
	              const std::string& skill,
	              const Coalition::Side& coalition = Coalition::Blue);

	[[nodiscard]] const std::string& Type() const { return m_type; }
	[[nodiscard]] const std::string& Name() const { return m_name; }
	[[nodiscard]] const std::string& Skill() const { return m_skill; }
	[[nodiscard]] const Coalition::Side& Coalition() const { return m_coalition; }

private:
	Coalition::Side m_coalition;
	std::string m_type, m_name, m_skill;
};
