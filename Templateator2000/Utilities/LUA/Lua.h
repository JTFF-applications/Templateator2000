#pragma once

namespace lua
{
	extern "C" {
	#include <lauxlib.h>
	#include "Lua.h"
	}
}

#include <nlohmann/json.hpp>

namespace json = nlohmann;

class Lua
{
public:
	static json::json JsonFromFile(const std::string& filepath, const std::string& table_name);
	static json::json JsonFromString(const std::string& content, const std::string& table_name);
	static json::json JsonFromConfigFile(const std::string& content, const std::string& table_name);

private:
	static bool CheckLua(lua::lua_State* l, int r);
};
