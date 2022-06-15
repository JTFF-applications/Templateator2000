#pragma once

#include <filesystem>

#include "Models/Tanker.h"

class Scripts
{
	friend class Mission;

public:
	Scripts();
	explicit Scripts(const std::filesystem::path& path);

	void Init(const std::filesystem::path& path);
	[[nodiscard]] const std::vector<Tanker>& Tankers() const { return m_tankers; }

private:
	void Load();

private:
	std::filesystem::path m_path;
	bool m_initialized;

	std::vector<std::string> m_scripts_installed;
	std::vector<Tanker> m_tankers;
};
