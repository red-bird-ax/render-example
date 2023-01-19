#include <Render/OpenGL/Assets/GL_Texture.h>
#include <Render/OpenGL/GL_Error.h>
#include <Core.h>

//#include <External/stb/stb_image.h>

#include <GL/glew.h>

#include <unordered_map>

namespace OpenGL
{
	const std::unordered_map<WrappingMode, int32_t> TextureWrapping
	{
		{ WrappingMode::Wrap,   GL_REPEAT },
		{ WrappingMode::Mirror, GL_MIRRORED_REPEAT },
		{ WrappingMode::Clamp,  GL_CLAMP_TO_EDGE },
		{ WrappingMode::Border, GL_CLAMP_TO_BORDER },
	};

	const std::unordered_map<FilteringMode, int32_t> TextureFiltering
	{
		{ FilteringMode::Linnear, GL_LINEAR },
		{ FilteringMode::Point,   GL_NEAREST  },
	};

	void Texture::Initialize(const uint8_t* buffer, int32_t width, int32_t  height, int32_t channels, const TextureParams& params)
	{
		m_Width    = width;
		m_Height   = height;
		m_Channels = channels;

		GL_CALL(glGenTextures(1, &m_Handle));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Handle));

		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapping.at(params.HorizontalWrapping)));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapping.at(params.VerticalWrapping)));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFiltering.at(params.Filtering)));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::Destroy()
	{
		GL_CALL(glDeleteTextures(1, &m_Handle));
	}

	void Texture::Bind(uint32_t slot) const
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Handle));
	}

	void Texture::Unbind() const
	{
		Texture::UnbindTextures();
	}

	void Texture::UnbindTextures()
	{
		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}
}
