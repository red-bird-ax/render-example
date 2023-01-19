#pragma once

#include <Render/OpenGL/Buffers/GL_VertexBuffer.h>
#include <Render/OpenGL/Buffers/GL_IndexBuffer.h>
#include <Render/OpenGL/Buffers/GL_VertexLayout.h>

#include <string>

namespace OpenGL
{
	class Mesh
	{
	public:
		void BuildFromBuffers(const VertexBuffer& vb, const IndexBuffer& ib, const VertexLayout& layout);
		
		void Destroy();

		void Bind() const;
		void Unbind() const;

		static void UnbindBuffers();

		inline void SetName(const std::string& name) { m_Name = name; }
		inline const std::string& GetName() const { return m_Name; }

		inline uint32_t GetIndicesCount() const { return m_IB.GetIndicesCount(); }
		inline uint32_t GetVerticesCount() const { return m_VB.GetVerticesCount(); }

	private:
		void BindLayout(const VertexLayout& layout);

	private:
		uint32_t m_VAHandle = 0;
		std::string m_Name = "Mesh";

		VertexBuffer m_VB;
		IndexBuffer  m_IB;
	};
}