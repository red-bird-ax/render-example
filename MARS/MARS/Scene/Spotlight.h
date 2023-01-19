#pragma once

#include <Scene/Scene.h>
#include <Scene/PointLight.h>

class Spotlight : public Actor
{
public:

	Vector3D GetLocation() const;
	Vector3D GetDirection() const;

	inline void GetRadiuses(float& inner, float& outer) const
	{
		inner = m_InnerRadius;
		outer = m_OuterRadius;
	}

private:
	float m_InnerRadius = 0.f;
	float m_OuterRadius = 0.f;

	LightProperties m_Properties;
};