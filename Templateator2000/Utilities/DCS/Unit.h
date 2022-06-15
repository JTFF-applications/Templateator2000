#pragma once

#include <string>

class Unit
{
public:
	explicit Unit(const std::string& type, const std::string& name);

	[[nodiscard]] const std::string& Type() const { return m_type; }
	[[nodiscard]] const std::string& Name() const { return m_name; }

private:
	std::string m_type, m_name;
};
