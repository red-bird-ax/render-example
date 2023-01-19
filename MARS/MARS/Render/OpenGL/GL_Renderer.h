#pragma once

#include <Window.h>

#include <SDL.h>

class Color;

namespace OpenGL
{
	class Renderer // TODO: implement IRenderer interface
	{
	public:
		void Initialize(Window& window);
		void Destroy();

		void BeginFrame(const Color& color);
		void EndFrame();

		void PrintInfo() const;

		void Test01();

	private:
		void InitilizeImGUI();
		void DestroyImGUI();

	private:
		Window* p_Window = nullptr;
		SDL_GLContext m_Context = nullptr;
		// Add ASPECT RATIO!!!!
	};
}