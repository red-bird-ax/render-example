#pragma once

#include <Scene/Scene.h>
#include <Math/Color.h>

// Uniform slots:
// - camera: 8
// - light:  9
// - object  0..7

struct LightProperties
{
	NormColor Ambient;
	NormColor Diffuse;
	NormColor Specular;
};

class PointLight : public Actor
{
public:
	PointLight(const std::string& name, const Transform& transform);
	virtual ActorType GetType() const override;
	virtual void DrawUI() override;

	static void Initailze(ID modelID, ID textureID);

	void Render();

	/*inline NormColor GetLightColor() const { return m_LightClolor; }
	inline void SetLightColor(const Color& color) { m_LightClolor = color.ToNormalized(); }*/
	
	/*inline float GetIntensity() const { return m_Intensity; }
	inline void SetIntensity(float intensity) { m_Intensity = intensity; }*/

	inline const LightProperties& GetProperties() const { return m_Properties; }
	inline void SetProperties(const LightProperties& props) { m_Properties = props; }

private:
	//NormColor m_LightClolor;
	//float m_Intensity = 1.f;
	
	LightProperties m_Properties;
	

	inline static ID m_ModelID   = 0;
	inline static ID m_TextureID = 0;
};