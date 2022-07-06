#include <filesystem>
#include <fstream>
#include <unordered_map>

#include "Utilities/Log.h"
#include "Utilities/LUA/Lua.h"
#include "Windows/InjectorWindow.h"
#include "Utilities/Scripting/Injector.h"

void Injector::ManageScripts(const libzippp::ZipArchive& archive, std::vector<std::string>& installed_scripts)
{
	// ReSharper disable StringLiteralTypo
	LOG_INFO("Starting scripts injection in {}", archive.getPath());

	const std::vector<std::string> was_installed = installed_scripts;
	bool is_cancelled = false;
	InjectorWindow window([&](const std::vector<std::string>& scripts)
	                      {
		                      installed_scripts = scripts;
	                      },
	                      [&]
	                      {
		                      is_cancelled = true;
	                      },
	                      installed_scripts);
	window.exec();
	if (is_cancelled)
	{
		LOG_INFO("Scripts injection cancelled");
		return;
	}

	updateSources(archive);
	updateLibraries(archive);
	addSounds(archive);

	const std::string mission_data_string = archive.getEntry("mission").readAsText();
	const std::string map_resource_string = archive.getEntry("l10n/DEFAULT/mapResource").readAsText();
	json::json mission_data = Lua::JsonFromLua(mission_data_string, "mission");
	json::json map_resource = Lua::JsonFromLua(map_resource_string, "mapResource");

	std::unordered_map<std::string, std::string> settings_files;

	// Inject basic scripts (Needed for all)
	if (was_installed.empty())
	{
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
		settings_files["HypemanConfig"] = "settings-hypeman.lua";

		// Inject SetClients
		injectOne(mission_data,
		          map_resource,
		          "Set Clients",
		          {"110-set_clients.lua"},
		          21,
		          "0xff0000ff");

		// Inject Installed Scripts
		injectOne(mission_data,
		          map_resource,
		          "DO NOT DELETE : Injected Scripts",
		          {"injected_scripts.lua"},
		          15,
		          "0x000000ff");
	}

	// Inject Atis
	if (std::ranges::find(installed_scripts, "atis") != installed_scripts.end() &&
	    std::ranges::find(was_installed, "atis") == was_installed.end())
	{
		injectOne(mission_data,
		          map_resource,
		          "Atis",
		          {"160-atis.lua"},
		          26,
		          "0xff0000ff");
		settings_files["AtisConfig"] = "settings-atis.lua";
	}

	// Inject A/A
	if (std::ranges::find(installed_scripts, "air_to_air") != installed_scripts.end() &&
	    std::ranges::find(was_installed, "air_to_air") == was_installed.end())
	{
		injectOne(mission_data,
		          map_resource,
		          "A/A",
		          {"170-cap_zone_training.lua", "173-fox_zone_training.lua"},
		          27,
		          "0xff0000ff");
		settings_files["TrainingCAPConfig"] = "settings-capzone.lua";
		settings_files["FoxRangesConfig"] = "settings-foxzone.lua";
	}

	// Inject A/G
	if (std::ranges::find(installed_scripts, "air_to_ground") != installed_scripts.end() &&
	    std::ranges::find(was_installed, "air_to_ground") == was_installed.end())
	{
		injectOne(mission_data,
		          map_resource,
		          "A/G",
		          {"190-ranges.lua", "193-training_ranges.lua", "196-fac_ranges.lua", "199-skynet.lua"},
		          28,
		          "0xff0000ff");
		settings_files["RangeConfig"] = "settings-ranges.lua";
		settings_files["TrainingRangeConfig"] = "settings-training_ranges.lua";
		settings_files["FACRangeConfig"] = "settings-fac_ranges.lua";
		settings_files["SkynetConfig"] = "settings-skynet.lua";
	}

	// Inject Airboss
	if (std::ranges::find(installed_scripts, "airboss") != installed_scripts.end() &&
	    std::ranges::find(was_installed, "airboss") == was_installed.end())
	{
		injectOne(mission_data,
		          map_resource,
		          "Airboss",
		          {"130-airboss.lua", "135-pedro.lua"},
		          23,
		          "0xff0000ff");
		settings_files["AirBossConfig"] = "settings-airboss.lua";
		settings_files["PedrosConfig"] = "settings-pedros.lua";
	}

	// Inject Awacs
	if (std::ranges::find(installed_scripts, "awacs") != installed_scripts.end() &&
	    std::ranges::find(was_installed, "awacs") == was_installed.end())
	{
		injectOne(mission_data,
		          map_resource,
		          "Awacs",
		          {"150-awacs.lua"},
		          25,
		          "0xff0000ff");
		settings_files["AwacsConfig"] = "settings-awacs.lua";
	}

	// Inject Beacons
	if (std::ranges::find(installed_scripts, "beacons") != installed_scripts.end() &&
	    std::ranges::find(was_installed, "beacons") == was_installed.end())
	{
		injectOne(mission_data,
		          map_resource,
		          "Beacons",
		          {"140-beacons.lua"},
		          24,
		          "0xff0000ff");
		settings_files["BeaconsConfig"] = "settings-beacons.lua";
	}

	// Inject Mission
	if (std::ranges::find(installed_scripts, "mission") != installed_scripts.end() &&
	    std::ranges::find(was_installed, "mission") == was_installed.end())
		injectOne(mission_data,
		          map_resource,
		          "Mission Specific",
		          {"180-mission.lua"},
		          27,
		          "0xff0000ff");

	//Inject Random Air Traffic
	if (std::ranges::find(installed_scripts, "random_air_traffic") != installed_scripts.end() &&
	    std::ranges::find(was_installed, "random_air_traffic") == was_installed.end())
	{
		injectOne(mission_data,
		          map_resource,
		          "Random Air Traffic",
		          {"176-random_air_traffic.lua"},
		          27,
		          "0xff0000ff");
		settings_files["RATConfig"] = "settings-RAT.lua";
	}

	// Inject Tankers
	if (std::ranges::find(installed_scripts, "tankers") != installed_scripts.end() &&
	    std::ranges::find(was_installed, "tankers") == was_installed.end())
	{
		injectOne(mission_data,
		          map_resource,
		          "Tankers",
		          {"120-tankers.lua"},
		          22,
		          "0xff0000ff");
		settings_files["TankersConfig"] = "settings-tankers.lua";
		settings_files["OnDemandTankersConfig"] = "settings-ondemandtankers.lua";
	}

	// Add settings file
	for (const auto& [name, file] : settings_files)
	{
		const std::string entry_path = std::format("l10n/DEFAULT/{}", file);
		const std::string temp_path = std::format("temp/{}", file);

		std::fstream file_stream(temp_path, std::ios::out);
		if (name == "HypemanConfig")
		{
			file_stream << "hypemanInstallPath = \"C:/hypeman-jtff-airboss/src\"\n";
			file_stream << "missionName = \"Autoscripted Mission By Templateator2000\"\n";
			file_stream.close();
		}
		else
			file_stream << name << " = {}";

		file_stream.close();
		addFile(archive, entry_path, temp_path);
	}

	// Delete old settings if needed
	if (!was_installed.empty())
	{
		deleteOne(mission_data,
		          map_resource,
		          "Mission Settings");
		settings_files["HypemanConfig"] = "settings-hypeman.lua";
	}

	// Inject settings
	std::vector<std::string> files;
	std::ranges::transform(settings_files,
	                       std::back_inserter(files),
	                       [](auto& kv) { return kv.second; });

	injectOne(mission_data,
	          map_resource,
	          "Mission Settings",
	          files,
	          15,
	          "0xffff00ff");

	// Save injected scripts into mission
	std::fstream injected_scripts_fs("temp/injected_scripts.lua", std::ios::out);
	injected_scripts_fs << Lua::LuaFromJson(installed_scripts, "InjectedScripts");
	injected_scripts_fs.close();
	addFile(archive, "l10n/DEFAULT/injected_scripts.lua", "temp/injected_scripts.lua");

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
	std::string destination = entry_name;
	std::ranges::replace(destination, '\\', '/');
	permissions(file, std::filesystem::perms::all);

	if (!archive.addFile(destination, file))
		throw std::exception(std::format("Failed to add/replace {}", file).c_str());
	LOG_TRACE("{} added or updated.", destination);
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
		map_resource[script_file] = script_file;
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

void Injector::deleteOne(json::json& mission_data,
                         json::json& map_resource,
                         const std::string& title)
{
	// ReSharper disable StringLiteralTypo

	// Get trigrule object for this title
	const auto& trigrule = std::ranges::find_if(mission_data["trigrules"],
	                                            [&title](const json::json& rule)
	                                            {
		                                            return rule["comment"] == title;
	                                            });

	std::vector<std::string> script_files;
	for (const auto& action_object : (*trigrule)["actions"])
	{
		const std::string& file = action_object["file"];
		if (map_resource.contains(file))
		{
			script_files.push_back(file);
			map_resource.erase(file);
		}
	}

	// Get action string (to erase mission_data["trig"]["actions"] and find next_index)
	const auto& action_string_it = std::ranges::find_if(mission_data["trig"]["actions"],
	                                                    [&script_files](const json::json& value)
	                                                    {
		                                                    return std::ranges::any_of(script_files,
			                                                    [&value](const std::string& file)
			                                                    {
				                                                    return value.get<std::string>().find(file) !=
				                                                           std::string::npos;
			                                                    });
	                                                    });

	const std::string& action_string = *action_string_it;
	const size_t first_slash = action_string.find_last_of('[') + 1;
	const size_t second_slash = action_string.find_last_of(']') - first_slash;

	// Get next index (to find mission_data["trig"]["func"])
	const int next_index = std::stoi(action_string.substr(first_slash, second_slash));

	// Get timing (to erase mission_data["trig"]["conditions"])
	const int timing = (*std::ranges::find_if((*trigrule)["rules"],
	                                          [](const json::json& rule)
	                                          {
		                                          return rule.contains("seconds");
	                                          }))["seconds"];

	// Erase all data from mission_data
	mission_data["trig"]["actions"].erase(action_string_it);
	mission_data["trig"]["func"].erase(std::ranges::find_if(mission_data["trig"]["func"],
	                                                        [&next_index](const json::json& value)
	                                                        {
		                                                        return value == std::format(
			                                                               "if mission.trig.conditions[{}]() then mission.trig.actions[{}]() end",
			                                                               next_index,
			                                                               next_index);
	                                                        }));
	mission_data["trig"]["conditions"].erase(std::ranges::find_if(mission_data["trig"]["conditions"],
	                                                              [&timing](const json::json& value)
	                                                              {
		                                                              return value.get<std::string>().find(std::format(
			                                                                     "c_time_after({})",
			                                                                     timing)) != std::string::npos;
	                                                              }));
	mission_data["trig"]["flag"].erase(--mission_data["trig"]["flag"].end());
	mission_data["trigrules"].erase(trigrule);
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
