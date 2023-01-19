#pragma once

#include <string>

namespace OpenGL
{
	struct UniformBlockInfo
	{
		uint32_t Index;
		int32_t  Size;
	};

	class UniformBuffer;

	class Shader
	{
	public:
		enum class Type : uint32_t
		{
			Vertex = 0x8B31,
			Pixel  = 0x8B30,
		};

	public:
		void Load(const std::string& path);
		void BuildProgram(const std::string& vsSource, const std::string& psSource);
		void Destroy();

		void Bind() const;
		void Unbind() const;

		static void UnbindProgram();

		void LinkUniformBuffer(const UniformBuffer& ub, const std::string& name);
		void LinkTexture(const std::string& name, uint32_t slot);


		UniformBlockInfo QueryUniformBlock(const std::string& uniformName) const;
		
	private:
		static uint32_t CompileShader(Shader::Type type, const std::string& source);
		
	private:
		uint32_t m_Handle = 0;
	};
}