#pragma once

#include <filesystem>

#include "Models/Tanker.h"

class Scripts
{
	friend class Mission;

public:
	Scripts();
	Scripts(const std::filesystem::path& path);
	~Scripts() = default;

	void Init(const std::filesystem::path& path);
	const std::vector<Tanker>& Tankers() const { return m_tankers; }

private:
	void Load();

private:
	std::filesystem::path m_path;
	bool m_initialized;

	std::vector<std::string> m_scripts_installed;
	std::vector<Tanker> m_tankers;
};
