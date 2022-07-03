#pragma once
#include <libzippp/libzippp.h>

class Injector
{
public:
	static void InjectScripts(const libzippp::ZipArchive& archive);

private:
	static void addFile(const libzippp::ZipArchive& archive,
	                    const std::string& entry_name,
	                    const std::string& file);
	static void addFiles(const libzippp::ZipArchive& archive,
	                     const std::string& extension,
	                     const std::filesystem::path& source_path,
	                     const std::filesystem::path& destination_path);
	static void addSounds(const libzippp::ZipArchive& archive);
	static void injectOne(json::json& mission_data,
	                      json::json& map_resource,
	                      const std::string& title,
	                      const std::vector<std::string>& script_files,
	                      const int& timing,
	                      const std::string& hex_color);
	static void updateLibraries(const libzippp::ZipArchive& archive);
	static void updateSources(const libzippp::ZipArchive& archive);
};
