#include <format>
#include <fstream>
#include <sstream>

#include "Utilities/Log.h"
#include "Utilities/LUA/Lua.h"

json::json Lua::JsonFromFile(const std::string& filepath, const std::string& table_name)
{
	const auto l = lua::luaL_newstate();

	std::stringstream buffer;
	std::ifstream mission(filepath);
	buffer << mission.rdbuf();
	std::ifstream parser("templates/mission_parser.lua");
	buffer << parser.rdbuf();

	auto lua_content = buffer.str();
	lua_content.replace(lua_content.find("__TABLE_TO_ENCODE__"), 19, table_name);

	if (CheckLua(l, luaL_dostring(l, lua_content.c_str())))
	{
		lua_getglobal(l, "str");
		if (lua_isstring(l, -1))
		{
			const std::string json = lua::lua_tostring(l, -1);
			return json::json::parse(json);
		}
		throw std::exception("Lua error : Can't find str in lua !");
	}
	return {};
}

json::json Lua::JsonFromString(const std::string& content, const std::string& table_name)
{
	const auto l = lua::luaL_newstate();

	std::stringstream buffer;
	buffer << content;
	const std::ifstream parser("templates/mission_parser.lua");
	buffer << parser.rdbuf();

	auto lua_content = buffer.str();
	lua_content.replace(lua_content.find("__TABLE_TO_ENCODE__"), 19, table_name);

	if (CheckLua(l, luaL_dostring(l, lua_content.c_str())))
	{
		lua_getglobal(l, "str");
		if (lua_isstring(l, -1))
		{
			const std::string json = lua::lua_tostring(l, -1);
			return json::json::parse(json);
		}
		throw std::exception("Lua error : Can't find str in lua !");
	}
	return {};
}

json::json Lua::JsonFromConfigFile(const std::string& content, const std::string& table_name)
{
	auto str = content;
	bool str_found = false;
	for (int i = 0; i < str.size(); ++i)
	{
		if (!str_found && str[i] == '=' && (std::isalpha(str[i + 1]) || std::isalpha(str[i + 2])))
		{
			str_found = true;
			str.insert(i + (std::isspace(str[i + 1]) ? 2 : 1), "\'");
		}

		if (str_found && str[i] == ',')
		{
			str_found = false;
			str.insert(i, "\'");
		}
	}

	return JsonFromString(str, table_name);
}

bool Lua::CheckLua(lua::lua_State* l, int r)
{
	if (r != 0)
	{
		auto err = lua_pcall(l, 0, 0, 0);
		const std::string error = lua::lua_tostring(l, -1);
		lua_pop(l, 1);
		throw std::exception(std::format("Lua error : {}", error).c_str());
	}
	return r == 0;
}
