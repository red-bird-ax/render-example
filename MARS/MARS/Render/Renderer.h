#pragma once

#include <Render/OpenGL/GL_Renderer.h>

using IRenderer = OpenGL::Renderer;

inline IRenderer* CreateRenderer() { return new IRenderer; }


enum class WrappingMode : uint8_t
{
	Wrap, Mirror, Clamp, Border,
};

enum class FilteringMode : uint8_t
{
	Linnear, Point,
};

struct TextureParams
{
	WrappingMode  HorizontalWrapping;
	WrappingMode  VerticalWrapping;
	FilteringMode Filtering;
};