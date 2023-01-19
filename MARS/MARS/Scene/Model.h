#pragma once

#include <Render/OpenGL/Buffers/GL_UniformBuffer.h>
#include <Scene/Scene.h>
#include <Math/Transform.h>

#include <Core.h>

#include <vector>
#include <string>

struct Material
{
	NormColor Ambient;
	NormColor Diffuse;
	NormColor Specular;
	float Shininess;
};

class Model : public Actor
{
private:
	struct UIData
	{
		//char RenameBuffer[256];
		int32_t SelectedAssetIdx = 0;
	};

public:
	Model(const std::string& name, const Transform& transform, ID assetID);
	virtual void DrawUI() override;
	virtual ActorType GetType() const override;
	
	void Render();
	inline void SetMaterial(const Material& mat) { m_Material = mat; }

private:
	ID m_MeshGruopID = 0;
	Material m_Material;
	std::vector<Transform> m_SubTransforms;

	OpenGL::UniformBuffer m_MeshUB;
	OpenGL::UniformBuffer m_MaterialUB;

	UIData m_UIData;
};