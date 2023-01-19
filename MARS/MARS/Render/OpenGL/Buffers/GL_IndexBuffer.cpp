#include <Render/OpenGL/Buffers/GL_IndexBuffer.h>
#include <Render/OpenGL/GL_Error.h>

namespace OpenGL
{
	void IndexBuffer::Initialize(uint32_t count, const IndexType* indices)
	{
		m_IndicesCount = count;

		GL_CALL(glGenBuffers(1, &m_Handle));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndicesCount * GetIndexSize(), indices, GL_STATIC_DRAW)); // TODO: do smth with GL_STATIC_DRAW

		IndexBuffer::UnbindBuffers();
	}

	void IndexBuffer::Destroy()
	{
		GL_CALL(glDeleteBuffers(1, &m_Handle));
		m_IndicesCount = 0;
	}

	void IndexBuffer::Bind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
	}

	void IndexBuffer::Unbind() const
	{
		IndexBuffer::UnbindBuffers();
	}

	void IndexBuffer::UnbindBuffers()
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}