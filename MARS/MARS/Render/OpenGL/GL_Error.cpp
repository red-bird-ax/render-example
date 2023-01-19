#include <Render/OpenGL/GL_Error.h>

#include <cstdio>
#include <cstdint>
#include <unordered_map>

const std::unordered_map<uint32_t, const char*> GLErrorMessages
{
	{ 0x0500, "invalid enumeration" },
	{ 0x0501, "invalid value" },
	{ 0x0502, "invalid operation" },
	{ 0x0503, "stack overflow" },
	{ 0x0504, "stack underflow" },
	{ 0x0505, "out of memory" },
	{ 0x0506, "invalid framebuffer operation" },
	{ 0x0507, "context lost" },
	{ 0x8031, "table too lagre" },
};

bool GLLogCall()
{
	while (uint32_t error = glGetError())
	{
		if (GLErrorMessages.contains(error))
		{
			printf("OpenGL error: %s(0x%x)\n", GLErrorMessages.at(error), error);
		}
		else
		{
			printf("Unknown OpenGL error: 0x%x\n", error);
		}
		return false;
	}
	return true;
}