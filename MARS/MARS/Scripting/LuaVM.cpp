#include <Scripting/LuaVM.h>

#include <Core.h>

#include <sstream>

void LuaVM::Intitalize()
{
	p_State = luaL_newstate();
	ASSERT(p_State != nullptr, "Failed to create Lua virtual machine");

	luaL_openlibs(p_State);

	

}

void LuaVM::Destroy()
{
	lua_close(p_State);
}

bool LuaVM::LuaCall(int32_t result, const std::string& message)
{
	if (result != LUA_OK)
	{
		std::stringstream stream;
		stream << "Lua Error";

		if (message != "")
		{
			stream << " [ " << message << " ]";
		}

		stream << ": " << lua_tostring(p_State, -1);
		return false;
	}
	return true;
}