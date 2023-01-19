#include <Math/Color.h>

NormColor Color::ToNormalized() const
{
	return NormColor
	{
		.R = R / 255.f,
		.G = G / 255.f,
		.B = B / 255.f,
		.A = A / 255.f,
	};
}

Color NormColor::ToColor() const
{
	return Color
	{
		.R = static_cast<uint8_t>(R * 255),
		.G = static_cast<uint8_t>(G * 255),
		.B = static_cast<uint8_t>(B * 255),
		.A = static_cast<uint8_t>(A * 255),
	};
}