#pragma once

#include <Scene/Scene.h>

#include <Render/OpenGL/GL_Shader.h>
#include <Render/OpenGL/Buffers/GL_UniformBuffer.h>

class Cube : public Actor
{
public:
	Cube(const std::string& name, const Transform& transform);

	virtual ActorType GetType() const override;
	virtual void DrawUI() override;

	void Render();
	
	inline void SetTextures(ID diffuse, ID specular, ID emission)
	{
		m_DiffuseTextureID  = diffuse;
		m_SpecularTextureID = specular;
		m_EmissionTextureID = emission;
	}

	static void InitializeResourses();

private:
	ID m_DiffuseTextureID  = 0;
	ID m_SpecularTextureID = 0;
	ID m_EmissionTextureID = 0;
	float m_Shininess      = 32.f;

	OpenGL::UniformBuffer m_MeshUB;
	OpenGL::UniformBuffer m_MaterialUB;

	struct {
		int32_t DiffuseIndex;
		int32_t SpecularIndex;
		int32_t EmissionIndex;
	} m_UIData = { 0, 0, 0 };

	inline static OpenGL::Shader s_Shader;
	inline static ID s_ModelID;
};

