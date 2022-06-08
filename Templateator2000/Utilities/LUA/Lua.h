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
	static json::json JsonFromFile(const std::string& filepath);
	static json::json JsonFromString(const std::string& content);

private:
	static bool CheckLua(lua::lua_State* L, int r);
};
