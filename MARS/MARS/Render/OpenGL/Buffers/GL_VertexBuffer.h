#pragma once

#include <Render/OpenGL/GL_Error.h>

#include <cstdint>

#include <GL/glew.h>

namespace OpenGL
{
	class VertexBuffer
	{
	public:
		template <typename V>
		void Initialize(uint32_t count, const V* vertices)
		{
			m_VerticesCount = count;
			m_VertexSize    = sizeof(V);

			GL_CALL(glGenBuffers(1, &m_Handle));
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
			GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_VerticesCount * m_VertexSize, vertices, GL_STATIC_DRAW)); // do smth about GL_STATIC_DRAW

			VertexBuffer::UnbindBuffers();
		}

		void Destroy();

		void Bind() const;
		void Unbind() const;

		static void UnbindBuffers();

		inline uint32_t GetVerticesCount() const { return m_VerticesCount; }
		inline uint32_t GetVertexSize()    const { return m_VertexSize; }

	private:
		uint32_t m_Handle        = 0;
		uint32_t m_VerticesCount = 0;
		uint32_t m_VertexSize    = 0;
	};
}