#include <Render/OpenGL/Assets/GL_MeshGroup.h>
#include <Math/Vector.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace OpenGL
{
	void MeshGroup::Initialize(const std::string& name, uint32_t count, Mesh* meshes)
	{
		m_Name   = name;
		m_Count  = count;
		p_Meshes = meshes;
	}

	void MeshGroup::Destroy()
	{
		for (uint32_t idx = 0; idx < m_Count; ++idx)
		{
			p_Meshes[idx].Destroy();
		}
		delete[] p_Meshes;
	}

	uint32_t MeshGroup::GetToltalVertices() const
	{
		uint32_t vertices = 0;
		for (uint32_t idx = 0; idx < m_Count; ++idx)
		{
			vertices += p_Meshes[idx].GetVerticesCount();
		}
		return vertices;
	}

	uint32_t MeshGroup::GetToltalFaces() const
	{
		uint32_t faces = 0;
		for (uint32_t idx = 0; idx < m_Count; ++idx)
		{
			faces += p_Meshes[idx].GetIndicesCount() / 3;
		}
		return faces;
	}
}
