#pragma once

#include <Render/Renderer.h>
#include <Math/Vector.h>

#include <string>

namespace OpenGL
{
	class Texture
	{
	public:
		void Initialize(const uint8_t* buffer, int32_t width, int32_t height, int32_t channels, const TextureParams& params);
		void Destroy();

		void Bind(uint32_t slot) const;
		void Unbind() const;

		static void UnbindTextures();

		inline Vector2D GetSize() const { return { static_cast<float>(m_Width), static_cast<float>(m_Height) }; }
		inline uint32_t GetChannelsNumber() const { return m_Channels; }

		inline void SetName(const std::string& name) { m_Name = name; }
		inline void SetPath(const std::string& path) { m_Path = path; }

		inline const std::string& GetName() const { return m_Name; }
		inline const std::string& GetPath() const { return m_Path; }

		inline uint32_t GetHandle() const { return m_Handle; } // for UI (do smth)

	private:
		uint32_t m_Handle   = 0;
		uint32_t m_Width    = 0;
		uint32_t m_Height   = 0;
		uint32_t m_Channels = 0;

		std::string m_Name;
		std::string m_Path;
	};
}