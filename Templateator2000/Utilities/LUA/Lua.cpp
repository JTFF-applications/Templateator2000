#include <filesystem>
#include <format>
#include <fstream>
#include <sstream>

#include "Utilities/Log.h"
#include "Utilities/LUA/Lua.h"

json::json Lua::JsonFromLua(const std::string& content, const std::string& table_name)
{
	lua::lua_State* l = lua::luaL_newstate();
	luaL_openlibs(l);

	std::stringstream buffer;
	buffer << getLibs() << "\n";
	buffer << content;
	buffer << "str = JSON:encode(" << table_name << ")";

	const auto lua_content = buffer.str();

	if (checkLua(l, luaL_dostring(l, lua_content.c_str())))
	{
		lua_getglobal(l, "str");
		if (lua_isstring(l, -1))
		{
			const std::string json = lua_tostring(l, -1);
			return json::json::parse(json);
		}
		throw std::exception("Lua error : Can't find str in lua !");
	}
	return {};
}

std::string Lua::LuaFromJson(const json::json& content, const std::string& table_name)
{
	lua::lua_State* l = lua::luaL_newstate();
	luaL_openlibs(l);

	std::stringstream exec_buffer;
	exec_buffer << getLibs() << "\n";
	exec_buffer << "json_content = JSON:decode([[\n" << content << "\n]])\n";
	exec_buffer << "str = SaveTable(json_content)\n";
	const auto lua_content = exec_buffer.str();

	if (checkLua(l, luaL_dostring(l, lua_content.c_str())))
	{
		lua_getglobal(l, "str");
		if (lua_isstring(l, -1))
			return std::format("{} = {}", table_name, lua_tostring(l, -1));
		throw std::exception("Lua error : Can't find str in lua !");
	}
	return "";
}

const std::string Lua::ExecuteString(const std::string& code, const std::string& value_name)
{
	return ExecuteRaw<std::string>(code,
	                               value_name,
	                               [&](lua::lua_State* l)
	                               {
		                               if (lua_isstring(l, -1))
			                               return lua_tostring(l, -1);
		                               throw std::runtime_error(
			                               "ASSERT NOT REACHED ! Lua::ExecuteRaw->Moose::GetCallsignFromNumber");
	                               });
}

int Lua::ExecuteInt(const std::string& code, const std::string& value_name)
{
	return ExecuteRaw<int>(code,
	                       value_name,
	                       [&](lua::lua_State* l)
	                       {
		                       if (lua_isinteger(l, -1))
			                       return static_cast<int>(lua_tointeger(l, -1));
		                       throw std::runtime_error(
			                       "ASSERT NOT REACHED ! Lua::ExecuteRaw->Moose::GetCallsignFromNumber");
	                       });
}

const std::string Lua::getLibs()
{
	std::stringstream result;
	for (const auto& file : {
		     "templateator_lua_lib.lua", "dcs_coalitions.lua", "moose_airbases.lua",
		     "moose_callsigns.lua"
	     })
	{
		const std::ifstream f(std::format("templates/{}", file));
		result << f.rdbuf() << "\n";
	}
	return result.str();
}

bool Lua::checkLua(lua::lua_State* l, const int& r)
{
	if (r != 0)
	{
		const int err = lua_pcall(l, 0, 0, 0);
		const std::string error = lua_tostring(l, -1);
		lua_pop(l, 1);
		throw std::exception(std::format("Lua error : {}, {}", err, error).c_str());
	}
	return r == 0;
}
