#pragma once

#include <Render/OpenGL/GL_Error.h>
#include <Core.h>

#include <GL/glew.h>

#include <string>

namespace OpenGL
{
	class UniformBuffer
	{
	public:
		void Initialize(uint32_t size, uint32_t slot = 0);
		void Destroy();

		void Bind() const;
		void Unbind() const;

		template <typename T>
		void Update(T* data)
		{
			uint32_t dataSize = sizeof(T);
			ASSERT(m_Size >= dataSize, "Provided data to large for uniform buffer");

			GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
			GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, data));
			GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		}

		inline uint32_t GetSlot() const { return m_Slot; }

	private:
		uint32_t m_Handle = 0;
		uint32_t m_Slot   = 0;
		uint32_t m_Size   = 0;
	};
}