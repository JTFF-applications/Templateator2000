#pragma once

#include <string>

class Unit
{
public:
	Unit(const std::string& type, const std::string& name);
	~Unit() = default;

	const std::string& Type() const { return m_type; }
	const std::string& Name() const { return m_name; }

private:
	std::string m_type, m_name;
};
