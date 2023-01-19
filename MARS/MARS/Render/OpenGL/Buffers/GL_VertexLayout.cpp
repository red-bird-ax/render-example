#include <Render/OpenGL/Buffers/GL_VertexLayout.h>
#include <Core.h>

#include <GL/glew.h>

namespace OpenGL
{
	LayoutElement::LayoutElement(DataType type)
	{
		switch (type)
		{
			case DataType::UnsignedByte:
			{
				NativeType = GL_UNSIGNED_BYTE;
				Size       = sizeof(uint8_t);
				Length     = 1;
				return;
			}

			case DataType::SignedByte:
			{
				NativeType = GL_BYTE;
				Size       = sizeof(int8_t);
				Length     = 1;
				return;
			}

			case DataType::UnsignedInteger:
			{
				NativeType = GL_UNSIGNED_INT;
				Size       = sizeof(uint32_t);
				Length     = 1;
				return;
			}

			case DataType::SignedInteger:
			{
				NativeType = GL_INT;
				Size       = sizeof(int32_t);
				Length     = 1;
				return;
			}

			case DataType::Float:
			{
				NativeType = GL_FLOAT;
				Size       = sizeof(float);
				Length     = 1;
				return;
			}

			case DataType::Float2:
			{
				NativeType = GL_FLOAT;
				Size = sizeof(float) * 2;
				Length = 2;
				return;
			}
			
			case DataType::Float3:
			{
				NativeType = GL_FLOAT;
				Size       = sizeof(float) * 3;
				Length     = 3;
				return;
			}

			case DataType::Float4:
			{
				NativeType = GL_FLOAT;
				Size       = sizeof(float) * 4;
				Length     = 4;
				return;
			}

			default: ASSERT(false, "Unknown data type");
		}
	}
}
