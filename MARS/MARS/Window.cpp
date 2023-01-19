#include <Window.h>
#include <Core.h>

#include <SDL.h>

void Window::Initialize(const Config& config)
{
	p_Handle = SDL_CreateWindow(
		config.Title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		config.Width,
		config.Height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	ASSERT(p_Handle != nullptr, "Failed to create window");
}

void Window::Destroy()
{
	SDL_DestroyWindow(p_Handle);
}

Vector2D Window::GetSize() const
{
	int32_t w, h;
	SDL_GetWindowSize(p_Handle, &w, &h);
	return Vector2D
	{
		.X = static_cast<float>(w),
		.Y = static_cast<float>(h),
	};
}