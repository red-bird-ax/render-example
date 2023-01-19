#pragma once

struct Vector2D
{
	float X = 0.f;
	float Y = 0.f;
};

struct Vector3D
{
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
};

inline Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs)
{
	return Vector3D
	{
		.X = lhs.X + rhs.X,
		.Y = lhs.Y + rhs.Y,
		.Z = lhs.Z + rhs.Z,
	};
}

inline Vector3D operator*(const Vector3D& lhs, const Vector3D& rhs)
{
	return Vector3D
	{
		.X = lhs.X * rhs.X,
		.Y = lhs.Y * rhs.Y,
		.Z = lhs.Z * rhs.Z,
	};
}

inline Vector3D operator+=(Vector3D& lhs, const Vector3D& rhs)
{
	lhs.X += rhs.X;
	lhs.Y += rhs.Y;
	lhs.Z += rhs.Z;
	return lhs;
}