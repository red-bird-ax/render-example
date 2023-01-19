#pragma once

#include <Render/OpenGL/Assets/GL_Mesh.h>

namespace OpenGL
{
	class MeshGroup
	{
	public:
		void Initialize(const std::string& name, uint32_t count, Mesh* meshes);
		void Destroy();

		inline std::string GetName() const { return m_Name; }

		inline void SetPath(const std::string& path) { m_Path = path; }
		inline const std::string& GetPath() const { return m_Path; }

		inline Mesh* GetMesh(uint32_t index) { return p_Meshes + index; }
		inline uint32_t GetLength() { return m_Count; }

		uint32_t GetToltalVertices() const;
		uint32_t GetToltalFaces() const;

		inline Mesh& operator[](uint32_t index) { return p_Meshes[index]; }
		inline const Mesh& operator[](uint32_t index) const { return p_Meshes[index]; }

		inline Mesh* begin() { return p_Meshes; }
		inline Mesh* end() { return p_Meshes + m_Count; };

		inline const Mesh* begin() const { return p_Meshes; }
		inline const Mesh* end() const { return p_Meshes + m_Count; };

	private:
		Mesh* p_Meshes  = nullptr;
		uint32_t m_Count = 0;

		std::string m_Name;
		std::string m_Path;
	};
}