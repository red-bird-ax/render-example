#pragma once

struct Rotator
{
	float Roll = 0.f;
	float Pitch = 0.f;
	float Yaw = 0.f;
};

inline Rotator operator+=(Rotator& lhs, const Rotator& rhs)
{
	lhs.Roll  += rhs.Roll;
	lhs.Pitch += rhs.Pitch;
	lhs.Yaw   += rhs.Yaw;
	return lhs;
}

inline Rotator operator+(const Rotator& lhs, const Rotator& rhs)
{
	return Rotator {
		.Roll  = lhs.Roll + rhs.Roll,
		.Pitch = lhs.Pitch + rhs.Pitch,
		.Yaw   = lhs.Yaw + rhs.Yaw,
	};
}