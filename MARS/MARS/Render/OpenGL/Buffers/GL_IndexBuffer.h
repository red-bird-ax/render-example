#pragma once

#include <cstdint>

#include <GL/glew.h>

namespace OpenGL
{
	const uint32_t NativeIndexType = GL_UNSIGNED_INT;
	using IndexType                = uint32_t;

	class IndexBuffer
	{
	public:
		void Initialize(uint32_t count, const IndexType* indices);
		void Destroy();

		void Bind() const;
		void Unbind() const;

		static void UnbindBuffers();

		inline uint32_t GetIndicesCount() const { return m_IndicesCount; }
		inline uint32_t GetIndexSize()    const { return sizeof(IndexType); }

	private:
		uint32_t m_Handle       = 0;
		uint32_t m_IndicesCount = 0;
	};
}