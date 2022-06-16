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
	void Save() const;
	[[nodiscard]] const std::vector<Tanker>& Tankers() const { return m_tankers; }

private:
	void Load();
	[[nodiscard]] const std::vector<std::string> GetInstalledScripts(const std::string& file_data) const;

private:
	std::filesystem::path m_path;
	bool m_initialized;

	std::vector<std::string> m_installed_scripts;
	std::vector<Tanker> m_tankers;
};
