#pragma once

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <string>

#pragma comment(lib, "liblua54.a")
class LuaVM
{
public:
	void Intitalize();
	void Destroy();

private:
	bool LuaCall(int32_t result, const std::string& message = "");

private:
	lua_State* p_State = nullptr;
};

