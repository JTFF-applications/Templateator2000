#pragma once

#include <pugixml.hpp>
namespace lua
{
	extern "C"
	{
#include "Lua.h"
#include "lauxlib.h"
#include "lualib.h"
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
	static bool CheckLua(lua::lua_State* L, int r);
};
