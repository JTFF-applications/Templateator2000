#pragma once

namespace lua
{
	extern "C" {
	#include <lauxlib.h>
	#include <lualib.h>
	}
}

#include <nlohmann/json.hpp>
namespace json = nlohmann;

class Lua
{
public:
	static json::json JsonFromLua(const std::string& content, const std::string& table_name);
	static std::string LuaFromJson(const json::json& content, const std::string& table_name);

	static const std::string ExecuteString(const std::string& code, const std::string& value_name);
	static int ExecuteInt(const std::string& code, const std::string& value_name);
	template<typename _Ty>
	static _Ty ExecuteRaw(const std::string& code,
	                      const std::string& value_name,
	                      const std::function<_Ty(lua::lua_State*)>& fn);

private:
	static const std::string getLibs();
	static bool checkLua(lua::lua_State* l, const int& r);
};

template<typename _Ty>
_Ty Lua::ExecuteRaw(const std::string& code,
                    const std::string& value_name,
                    const std::function<_Ty(lua::lua_State*)>& fn)
{
	lua::lua_State* l = lua::luaL_newstate();
	luaL_openlibs(l);

	std::stringstream exec_buffer;
	exec_buffer << getLibs() << "\n";
	exec_buffer << code << "\n";

	const auto lua_content = exec_buffer.str();

	if (checkLua(l, luaL_dostring(l, lua_content.c_str())))
	{
		lua_getglobal(l, value_name.c_str());
		return std::invoke(fn, l);
	}
	throw std::exception("UNKNOWN ERROR ! ASSERT NOT REACHED !");
}
