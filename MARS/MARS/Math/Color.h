#pragma once

#include <cstdint>

struct NormColor;

struct Color
{
	uint8_t R = 0;
	uint8_t G = 0;
	uint8_t B = 0;
	uint8_t A = 255;

	NormColor ToNormalized() const;
};

struct NormColor
{
	float R = 0.f;
	float G = 0.f;
	float B = 0.f;
	float A = 1.f;

	Color ToColor() const;
};