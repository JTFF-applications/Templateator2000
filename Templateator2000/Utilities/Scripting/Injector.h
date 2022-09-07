#pragma once
#include <libzippp/libzippp.h>

class Injector
{
public:
	static void ManageScripts(const libzippp::ZipArchive& archive, std::vector<std::string>& installed_scripts);
	static void UpdateScripts(const libzippp::ZipArchive& archive);

private:
	static void addFile(const libzippp::ZipArchive& archive,
	                    const std::string& entry_name,
	                    const std::string& file);
	static void addFiles(const libzippp::ZipArchive& archive,
	                     const std::string& extension,
	                     const std::filesystem::path& source_path,
	                     const std::filesystem::path& destination_path);
	static void addSounds(const libzippp::ZipArchive& archive,
	                      const std::string& folder);
	static void injectOne(json::json& mission_data,
	                      json::json& map_resource,
	                      const std::string& title,
	                      const std::vector<std::string>& script_files,
	                      const int& timing,
	                      const std::string& hex_color);
	static void deleteOne(json::json& mission_data,
	                      json::json& map_resource,
	                      const std::string& title);
	static void updateLibraries(const libzippp::ZipArchive& archive);
	static void updateSources(const libzippp::ZipArchive& archive);
};
