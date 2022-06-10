#include <format>
#include <fstream>
#include <sstream>

#include "Utilities/Log.h"
#include "Lua.h"

json::json Lua::JsonFromFile(const std::string& filepath, const std::string& table_name)
{
	lua::lua_State* l = lua::luaL_newstate();
	lua::luaL_openlibs(l);

	std::stringstream buffer;
	std::ifstream mission(filepath);
	buffer << mission.rdbuf();
	std::ifstream parser("templates/mission_parser.lua");
	buffer << parser.rdbuf();

	std::string lua_content = buffer.str();
	lua_content.replace(lua_content.find("__TABLE_TO_ENCODE__"), 19, table_name);

	int ret = lua::luaL_loadstring(l, lua_content.c_str()) || lua_pcall(l, 0, LUA_MULTRET, 0);
	if (CheckLua(l, ret))
	{
		lua::lua_getglobal(l, "str");
		if (lua::lua_isstring(l, -1))
		{
			const std::string json = lua::lua_tostring(l, -1);
			return json::json::parse(json);
		}
		throw std::exception("Lua error : Can't find str in lua !");
	}
	return json::json();
}

json::json Lua::JsonFromString(const std::string& content, const std::string& table_name)
{
	lua::lua_State* l = lua::luaL_newstate();
	lua::luaL_openlibs(l);

	std::stringstream buffer;
	buffer << content;
	std::ifstream parser("templates/mission_parser.lua");
	buffer << parser.rdbuf();

	std::string lua_content = buffer.str();
	lua_content.replace(lua_content.find("__TABLE_TO_ENCODE__"), 19, table_name);

	int ret = lua::luaL_loadstring(l, lua_content.c_str()) || lua_pcall(l, 0, LUA_MULTRET, 0);
	if (CheckLua(l, ret))
	{
		lua::lua_getglobal(l, "str");
		if (lua::lua_isstring(l, -1))
		{
			const std::string json = lua::lua_tostring(l, -1);
			return json::json::parse(json);
		}
		throw std::exception("Lua error : Can't find str in lua !");
	}
	return json::json();
}

json::json Lua::JsonFromConfigFile(const std::string& content, const std::string& table_name)
{
	std::string str = content;
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

	return Lua::JsonFromString(str, table_name);
}

bool Lua::CheckLua(lua::lua_State* L, int r)
{
	if (r != 0)
	{
		auto err = lua_pcall(L, 0, 0, 0);
		const std::string error = lua::lua_tostring(L, -1);
		lua_pop(L, 1);
		throw std::exception(std::format("Lua error : {}", error).c_str());
	}
	return r == 0;
}
