#pragma once

#include <Scene/PointLight.h>


class DirectionalLight : public Actor
{
public:
	DirectionalLight(const std::string& name, const Transform& transform);

	virtual ActorType GetType() const override;
	virtual void DrawUI() override;

	inline const LightProperties& GetProperties() const { return m_Properties; }
	inline void SetProperties(const LightProperties& props) { m_Properties = props; }

	Vector3D GetDirection() const;

private:
	LightProperties m_Properties;
};

