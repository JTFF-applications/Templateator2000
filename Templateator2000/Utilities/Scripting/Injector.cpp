#include <filesystem>
#include <fstream>
#include <unordered_map>

#include "Utilities/Log.h"
#include "Utilities/LUA/Lua.h"
#include "Utilities/Scripting/Injector.h"

void Injector::InjectScripts(const libzippp::ZipArchive& archive)
{
	// ReSharper disable StringLiteralTypo
	LOG_INFO("Starting scripts injection in {}", archive.getPath());

	updateSources(archive);
	updateLibraries(archive);
	addSounds(archive);

	const std::string mission_data_string = archive.getEntry("mission").readAsText();
	const std::string map_resource_string = archive.getEntry("l10n/DEFAULT/mapResource").readAsText();
	json::json mission_data = Lua::JsonFromLua(mission_data_string, "mission");
	json::json map_resource = Lua::JsonFromLua(map_resource_string, "mapResource");

	std::fstream injected_scripts("temp/scripts.txt", std::ios::out);
	std::unordered_map<std::string, std::string> settings_files;
	// Inject Moose
	injectOne(mission_data,
	          map_resource,
	          "Moose Load",
	          {"Moose_.lua"},
	          10,
	          "0x008000ff");

	// Inject Mist
	injectOne(mission_data,
	          map_resource,
	          "Mist Load",
	          {"mist_4_5_107.lua"},
	          13,
	          "0x008000ff");

	// Inject Skynet
	injectOne(mission_data,
	          map_resource,
	          "Skynet Load",
	          {"skynet-iads-compiled.lua"},
	          15,
	          "0x008000ff");

	// Inject JTFF Libraries
	injectOne(mission_data,
	          map_resource,
	          "JTFF Libraries Load",
	          {"010-root_menus.lua", "020-mission_functions.lua", "hypeman.lua"},
	          16,
	          "0xffff00ff");

	// Inject SetClients
	injectOne(mission_data,
	          map_resource,
	          "Set Clients",
	          {"110-set_clients.lua"},
	          21,
	          "0xff0000ff");
	injected_scripts << "clients\n";

	// Inject Tankers
	injectOne(mission_data,
	          map_resource,
	          "Tankers",
	          {"120-tankers.lua"},
	          22,
	          "0xff0000ff");
	settings_files["TankersConfig"] = "settings-tankers.lua";
	settings_files["OnDemandTankersConfig"] = "settings-ondemandtankers.lua";
	injected_scripts << "tankers\n";

	// Inject Airboss
	injectOne(mission_data,
	          map_resource,
	          "Airboss",
	          {"130-airboss.lua", "135-pedro.lua"},
	          23,
	          "0xff0000ff");
	settings_files["AirBossConfig"] = "settings-airboss.lua";
	settings_files["PedrosConfig"] = "settings-pedros.lua";
	injected_scripts << "airboss\n";

	// Inject Beacons
	injectOne(mission_data,
	          map_resource,
	          "Beacons",
	          {"140-beacons.lua"},
	          24,
	          "0xff0000ff");
	settings_files["BeaconsConfig"] = "settings-beacons.lua";
	injected_scripts << "beacons\n";

	// Inject Awacs
	injectOne(mission_data,
	          map_resource,
	          "Awacs",
	          {"150-awacs.lua"},
	          25,
	          "0xff0000ff");
	settings_files["AwacsConfig"] = "settings-awacs.lua";
	injected_scripts << "awacs\n";

	// Inject Atis
	injectOne(mission_data,
	          map_resource,
	          "Atis",
	          {"160-atis.lua"},
	          26,
	          "0xff0000ff");
	settings_files["AtisConfig"] = "settings-atis.lua";
	injected_scripts << "atis\n";

	// Inject Mission
	injectOne(mission_data,
	          map_resource,
	          "Mission Specific",
	          {"180-mission.lua"},
	          27,
	          "0xff0000ff");
	injected_scripts << "mission\n";

	// Add settings file
	for (const auto& [name, file] : settings_files)
	{
		const std::string entry_path = std::format("l10n/DEFAULT/{}", file);
		const std::string temp_path = std::format("temp/{}", file);

		std::fstream file_stream(temp_path, std::ios::out);
		file_stream << name << " = {}";
		file_stream.close();

		addFile(archive, entry_path, temp_path);
	}

	// Inject settings
	std::vector<std::string> files(settings_files.size());
	std::ranges::transform(settings_files,
	                       std::back_inserter(files),
	                       [](auto& kv) { return kv.second; });
	injectOne(mission_data,
	          map_resource,
	          "Mission Settings",
	          files,
	          15,
	          "0xffff00ff");

	injected_scripts.close();
	addFile(archive, "scripts.txt", "temp/scripts.txt");

	// Save injected mission files
	const std::string mission = Lua::LuaFromJson(mission_data, "mission");
	const std::string resources = Lua::LuaFromJson(map_resource, "mapResource");

	std::fstream mission_stream("temp/mission.lua", std::ios::out);
	std::fstream resources_stream("temp/mapResource.lua", std::ios::out);
	mission_stream << mission;
	resources_stream << resources;
	mission_stream.close();
	resources_stream.close();

	addFile(archive, "mission", "temp/mission.lua");
	addFile(archive, "l10n/DEFAULT/mapResource", "temp/mapResource.lua");

	LOG_INFO("Scripts injection completed without error !");
	// ReSharper restore StringLiteralTypo
}

void Injector::addFile(const libzippp::ZipArchive& archive,
                       const std::string& entry_name,
                       const std::string& file)
{
	if (!archive.addFile(entry_name, file))
		throw std::exception(std::format("Failed to add/replace {}", file).c_str());
	LOG_TRACE("{} added or updated.", file);
}

void Injector::addFiles(const libzippp::ZipArchive& archive,
                        const std::string& extension,
                        const std::filesystem::path& source_path,
                        const std::filesystem::path& destination_path)
{
	std::filesystem::recursive_directory_iterator begin_iterator(std::filesystem::path(source_path).make_preferred());
	const std::filesystem::recursive_directory_iterator end_iterator;
	std::error_code ec;
	while (begin_iterator != end_iterator)
	{
		const auto& path = begin_iterator->path();
		const auto filename = path.filename().string();
		if (!begin_iterator->is_directory() && extension == "*" || path.extension().string() == extension)
			addFile(archive, std::format("{}/{}", destination_path.string(), filename), path.string());

		begin_iterator.increment(ec);
		if (ec)
			throw std::exception(
				std::format("Error accessing {} : {}", begin_iterator->path().string(), ec.message()).c_str());
	}
}

void Injector::addSounds(const libzippp::ZipArchive& archive)
{
	const int err = archive.deleteEntry("General/");
	if (err != LIBZIPPP_ERROR_INVALID_PARAMETER && err < 0)
		throw std::exception("Failed to delete 'General' folder in mission.");

	std::filesystem::recursive_directory_iterator begin_iterator(
		std::filesystem::path("jtff-templates/resources/sounds").make_preferred());
	const std::filesystem::recursive_directory_iterator end_iterator;
	std::error_code ec;
	while (begin_iterator != end_iterator)
	{
		const auto& path = begin_iterator->path();
		if (!begin_iterator->is_directory())
		{
			const size_t sound_index = path.string().find("sounds\\");
			if (sound_index == std::string::npos)
				throw std::exception("Error while adding sounds : invalid path.");

			const std::string destination = path.string().substr(sound_index + 7);
			addFile(archive, destination, path.string());
		}

		begin_iterator.increment(ec);
		if (ec)
			throw std::exception(
				std::format("Error accessing {} : {}", begin_iterator->path().string(), ec.message()).c_str());
	}
}

void Injector::injectOne(json::json& mission_data,
                         json::json& map_resource,
                         const std::string& title,
                         const std::vector<std::string>& script_files,
                         const int& timing,
                         const std::string& hex_color)
{
	// ReSharper disable StringLiteralTypo
	if (!mission_data.contains("trig"))
		mission_data += {"trig", {}};

	size_t next_index = mission_data["trigrules"].size() + 1;
	if (next_index == 1)
	{
		mission_data["trig"] += {"actions", {}};
		mission_data["trig"] += {"func", {}};
		mission_data["trig"] += {"conditions", {}};
		mission_data["trig"] += {"flag", {}};
		if (mission_data.contains("trigrules"))
			mission_data["trigrules"].clear();
		else
			mission_data += {"trigrules", {}};
	}

	std::string action_string;
	json::json action_object = {};

	for (const auto& script_file : script_files)
	{
		action_string += std::format("a_do_script_file(getValueResourceByKey({})); ", script_file);
		action_object += {
			{"file", script_file},
			{"predicate", "a_do_script_file"}
		};
		map_resource += {script_file, script_file};
	}

	action_string += std::format("mission.trig.func[{}]=nil;", next_index);
	mission_data["trig"]["actions"] += action_string;
	mission_data["trig"]["func"] += std::format(
		"if mission.trig.conditions[{}]() then mission.trig.actions[{}]() end",
		next_index,
		next_index);
	mission_data["trig"]["conditions"] += std::format("return(c_time_after({}))", timing);
	mission_data["trig"]["flag"] += true;
	mission_data["trigrules"] += {
		{
			"rules", {
				{
					{"coalitionlist", "red"},
					{"seconds", timing},
					{"predicate", "c_time_after"},
					{"zone", ""},
				}
			}
		},
		{"eventlist", ""},
		{"comment", title},
		{"actions", action_object},
		{"predicate", "triggerOnce"},
		{"colorItem", hex_color},
	};
	// ReSharper restore StringLiteralTypo
}

void Injector::updateLibraries(const libzippp::ZipArchive& archive)
{
	addFiles(archive, ".lua", "jtff-templates/lib", "l10n/DEFAULT");
}

void Injector::updateSources(const libzippp::ZipArchive& archive)
{
	addFiles(archive, ".lua", "jtff-templates/src", "l10n/DEFAULT");
}
