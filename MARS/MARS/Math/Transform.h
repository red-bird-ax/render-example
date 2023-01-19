#pragma once

#include <Math/Vector.h>
#include <Math/Rotator.h>

#include <glm/glm.hpp>

using Matrix = glm::mat4;

struct Transform
{
	Vector3D Location = { 0.f, 0.f, 0.f };
	Rotator  Rotation = { 0.f, 0.f, 0.f };
	Vector3D Scale    = { 1.f, 1.f, 1.f };

	Matrix GetModelMatrix() const;
};

inline Transform operator+(const Transform& lhs, const Transform& rhs)
{
	return Transform{
		.Location = lhs.Location + rhs.Location,
		.Rotation = lhs.Rotation + rhs.Rotation,
		.Scale = lhs.Scale * rhs.Scale,
	};
}