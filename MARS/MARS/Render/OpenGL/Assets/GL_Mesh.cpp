#include <Render/OpenGL/Assets/GL_Mesh.h>

#include <GL/glew.h>

namespace OpenGL
{
	void Mesh::BuildFromBuffers(const VertexBuffer& vb, const IndexBuffer& ib, const VertexLayout& layout)
	{
		m_VB = vb;
		m_IB = ib;

		GL_CALL(glGenVertexArrays(1, &m_VAHandle));
		GL_CALL(glBindVertexArray(m_VAHandle));

		m_VB.Bind();
		BindLayout(layout);

		Mesh::UnbindBuffers();
	}

	void Mesh::BindLayout(const VertexLayout& layout)
	{
		uint32_t index  = 0;
		uint32_t offset = 0;
		uint32_t stride = 0;

		for (const LayoutElement& element : layout)
		{
			stride += element.Size;
		}

		for (const LayoutElement& element : layout)
		{	
			GL_CALL(glEnableVertexAttribArray(index));
			GL_CALL(glVertexAttribPointer(index, element.Length, element.NativeType, false, stride, (const void*)offset));

			offset += element.Size;
			index  += 1;
		}
	}

	void Mesh::Destroy()
	{
		GL_CALL(glDeleteVertexArrays(1, &m_VAHandle));
		m_VB.Destroy();
		m_IB.Destroy();
	}

	void Mesh::Bind() const
	{
		GL_CALL(glBindVertexArray(m_VAHandle));
		m_IB.Bind();
	}

	void Mesh::Unbind() const
	{
		Mesh::UnbindBuffers();
	}

	void Mesh::UnbindBuffers()
	{
		GL_CALL(glBindVertexArray(0));
		IndexBuffer::UnbindBuffers();
	}
}
