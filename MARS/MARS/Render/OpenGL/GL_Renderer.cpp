#include <Render/OpenGL/GL_Renderer.h>
#include <Render/OpenGL/GL_Error.h>

#include <External/imgui/imgui.h>
#include <External/imgui/imgui_impl_sdl.h>
#include <External/imgui/imgui_impl_opengl3.h>

#include <Math/Vector.h>
#include <Math/Color.h>

#include <Core.h>

#include <GL/glew.h>
#include <GL/GL.h>

#include <glm/glm.hpp>

#pragma comment(lib, "opengl32.lib")
namespace OpenGL
{
	void Renderer::Initialize(Window& window)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		p_Window = &window;
		m_Context = SDL_GL_CreateContext(p_Window->GetHandle());
		ASSERT(m_Context != nullptr, "Failed to create Open GL context");

		auto result = glewInit();
		ASSERT(result == GLEW_OK, glewGetErrorString(result));

		const Vector2D winSize = p_Window->GetSize();
		GL_CALL(glViewport(0, 0, winSize.X, winSize.Y));

		GL_CALL(glEnable(GL_BLEND));
		GL_CALL(glEnable(GL_DEPTH_TEST));
		GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		InitilizeImGUI();
		PrintInfo();
	}

	void Renderer::Destroy()
	{
		DestroyImGUI();
		SDL_GL_DeleteContext(m_Context);
	}

	void Renderer::BeginFrame(const Color& color)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		

		const auto [r, g, b, a] = color.ToNormalized();
		GL_CALL(glClearColor(r, g, b, a));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void Renderer::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(p_Window->GetHandle());
	}

	void Renderer::PrintInfo() const
	{
		printf("Open GL:\n");

		const unsigned char* gpu = glGetString(GL_RENDERER);
		printf("  %s\n", gpu);

		const unsigned char* lang = glGetString(GL_SHADING_LANGUAGE_VERSION);
		printf("  Language GLSL %s\n", lang);

		int32_t maj, min;
		GL_CALL(glGetIntegerv(GL_MAJOR_VERSION, &maj));
		GL_CALL(glGetIntegerv(GL_MINOR_VERSION, &min));
		printf("  Version  %d.%d\n", maj, min);

		int32_t profileMask;
		GL_CALL(glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profileMask));
		if (profileMask & GL_CONTEXT_CORE_PROFILE_BIT)
		{
			printf("  Core profile\n");
		}
		if (profileMask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
		{
			printf("  Compatibility profile\n");
		}
	}

	void Renderer::InitilizeImGUI()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplSDL2_InitForOpenGL(p_Window->GetHandle(), m_Context);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void Renderer::DestroyImGUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}
}