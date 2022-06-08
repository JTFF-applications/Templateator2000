#include <format>
#include <fstream>
#include <sstream>

#include "Utilities/Log.h"
#include "Lua.h"

json::json Lua::JsonFromFile(const std::string& filepath)
{
	lua::lua_State* l = lua::luaL_newstate();
	lua::luaL_openlibs(l);

	std::stringstream buffer;
	std::ifstream mission(filepath);
	buffer << mission.rdbuf();
	std::ifstream parser("templates/mission_parser.lua");
	buffer << parser.rdbuf();

	int ret = lua::luaL_loadstring(l, buffer.str().c_str()) || lua_pcall(l, 0, LUA_MULTRET, 0);
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

json::json Lua::JsonFromString(const std::string& content)
{
	lua::lua_State* l = lua::luaL_newstate();
	lua::luaL_openlibs(l);

	std::stringstream buffer;
	buffer << content;
	std::ifstream parser("templates/mission_parser.lua");
	buffer << parser.rdbuf();

	int ret = lua::luaL_loadstring(l, buffer.str().c_str()) || lua_pcall(l, 0, LUA_MULTRET, 0);
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
