#pragma once

#include <GL/glew.h>

bool GLLogCall();

#define GL_CALL(x) \
{ \
	while (glGetError() != GL_NO_ERROR); \
	x;\
	if (!(GLLogCall())) __debugbreak(); \
}