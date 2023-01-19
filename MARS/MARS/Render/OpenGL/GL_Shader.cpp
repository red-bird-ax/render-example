#include <Render/OpenGL/GL_Shader.h>
#include <Render/OpenGL/GL_Error.h>
#include <Render/OpenGL/Buffers/GL_UniformBuffer.h>

#include <Utility/FileUtility.h>
#include <Core.h>

#include <GL/glew.h>

namespace OpenGL
{
	const std::string VSToken = "Vertex Shader: ";
	const std::string PSToken = "Pixel Shader: ";

	void Shader::Load(const std::string& path)
	{
		const std::string composedShader = ReadText(path);

		const int32_t vsStart = composedShader.find(VSToken);
		const int32_t psStart = composedShader.find(PSToken);

		const std::string vsSource = composedShader.substr(vsStart + VSToken.size(), psStart - PSToken.size() - 1);
		const std::string psSource = composedShader.substr(psStart + PSToken.size(), vsStart - composedShader.size() - 1);

		BuildProgram(vsSource, psSource);
	}

	void Shader::BuildProgram(const std::string& vsSource, const std::string& psSource)
	{
		uint32_t vs = CompileShader(Type::Vertex, vsSource);
		uint32_t ps = CompileShader(Type::Pixel,  psSource);

		m_Handle = glCreateProgram();
		ASSERT(m_Handle != 0, "Failed to allocate shader program");

		GL_CALL(glAttachShader(m_Handle, vs));
		GL_CALL(glAttachShader(m_Handle, ps));
		GL_CALL(glLinkProgram(m_Handle));
		GL_CALL(glValidateProgram(m_Handle));

#ifndef _DEBUG
		GL_CALL(glDetachShader(m_Handle, vs));
		GL_CALL(glDetachShader(m_Handle, ps));
#endif // _DEBUG

		GL_CALL(glDeleteShader(vs));
		GL_CALL(glDeleteShader(ps));

		Shader::UnbindProgram();
	}

	void Shader::Destroy()
	{
		glDeleteProgram(m_Handle);
	}

	void Shader::Bind() const
	{
		GL_CALL(glUseProgram(m_Handle));
	}

	void Shader::Unbind() const
	{
		Shader::UnbindProgram();
	}

	void Shader::UnbindProgram()
	{
		GL_CALL(glUseProgram(0));
	}

	uint32_t Shader::CompileShader(Shader::Type type, const std::string& source)
	{
		uint32_t shaderHandle = glCreateShader(static_cast<uint32_t>(type));
		ASSERT(shaderHandle != 0, "Failed to allocate shader");

		const char* rawSource = source.c_str();
		GL_CALL(glShaderSource(shaderHandle, 1, &rawSource, nullptr));
		GL_CALL(glCompileShader(shaderHandle));

		int32_t compileResult;
		GL_CALL(glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileResult));
		if (compileResult == 0)
		{
			int32_t length;
			GL_CALL(glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length));

			char* message = (char*)alloca(length * sizeof(char));
			GL_CALL(glGetShaderInfoLog(shaderHandle, length, &length, message));
			ASSERT(false, message);

			glDeleteShader(shaderHandle);
			return 0;
		}

		return shaderHandle;
	}

	UniformBlockInfo Shader::QueryUniformBlock(const std::string& uniformName) const
	{
		UniformBlockInfo info;
		info.Index = glGetUniformBlockIndex(m_Handle, uniformName.c_str());
		GL_CALL(glGetActiveUniformBlockiv(m_Handle, info.Index, GL_UNIFORM_BLOCK_DATA_SIZE, &info.Size));

		return info;
	}

	void Shader::LinkUniformBuffer(const UniformBuffer& ub, const std::string& name)
	{
		const UniformBlockInfo info = QueryUniformBlock(name);
		GL_CALL(glUniformBlockBinding(m_Handle, info.Index, ub.GetSlot()));
	}

	void Shader::LinkTexture(const std::string& name, uint32_t slot)
	{
		GL_CALL(glUniform1i(glGetUniformLocation(m_Handle, name.c_str()), slot));
	}
}