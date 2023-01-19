#include <Render/OpenGL/Buffers/GL_VertexBuffer.h>

namespace OpenGL
{
	void VertexBuffer::Destroy()
	{
		GL_CALL(glDeleteBuffers(1, &m_Handle));
		m_VerticesCount = 0;
		m_VertexSize    = 0;
	}

	void VertexBuffer::Bind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
	}

	void VertexBuffer::Unbind() const
	{
		VertexBuffer::UnbindBuffers();
	}

	void VertexBuffer::UnbindBuffers()
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}