#pragma once

#include <filesystem>

#include "Models/Atis.h"
#include "Models/Beacon.h"
#include "Models/Carrier.h"
#include "Models/Tanker.h"

class Scripts
{
	friend class Mission;

public:
	Scripts();
	explicit Scripts(const std::filesystem::path& path);

	void Init(const std::filesystem::path& path);
	void Save() const;
	void Manage();
	[[nodiscard]] const std::vector<Tanker>& GetTankers() const { return m_tankers; }
	[[nodiscard]] const std::vector<Atis>& GetAtis() const { return m_atis; }
	[[nodiscard]] const std::vector<Beacon>& GetBeacons() const { return m_beacons; }
	[[nodiscard]] const std::vector<Carrier>& GetCarriers() const { return m_carriers; }

private:
	void load();
	[[nodiscard]] static const std::vector<std::string> installedScripts(const std::string& file_data);

private:
	std::filesystem::path m_path;
	bool m_initialized;

	std::vector<std::string> m_installed_scripts;
	std::vector<Tanker> m_tankers;
	std::vector<Atis> m_atis;
	std::vector<Beacon> m_beacons;
	std::vector<Carrier> m_carriers;
};
