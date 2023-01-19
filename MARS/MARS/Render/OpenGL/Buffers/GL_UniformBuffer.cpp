#include <Render/OpenGL/Buffers/GL_UniformBuffer.h>
#include <Math/Color.h>

namespace OpenGL
{
	void UniformBuffer::Initialize(uint32_t size, uint32_t slot)
	{
		m_Size = size;
		m_Slot = slot;

		// Allocate buffer:
		GL_CALL(glGenBuffers(1, &m_Handle));
		GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
		GL_CALL(glBufferData(GL_UNIFORM_BUFFER, m_Size, nullptr, GL_STATIC_DRAW)); // not really static!

		// Bind to slot:
		GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_Handle));

		// Unbind from pipeline:
		GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void UniformBuffer::Destroy()
	{
		GL_CALL(glDeleteBuffers(1, &m_Handle));
		m_Slot = 0;
		m_Size = 0;
	}

	void UniformBuffer::Bind() const
	{
		GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, m_Slot, m_Handle));
		GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
	}

	void UniformBuffer::Unbind() const
	{
		GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, m_Slot, 0));
		GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}
}
