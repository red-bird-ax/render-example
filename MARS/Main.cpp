#include <Application.h>
#include <Core.h>

#include <SDL.h>

#include <cstdlib>
#include <ctime>

#undef main
int main()
{
	srand(time(nullptr));
	{
		auto result = SDL_Init(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC));
		ASSERT(result == 0, "Failed to initialize SDL");
	}

	const Application::Config config
	{
		.Window{
			.Width  = 1920,
			.Height = 1080,
			.Title  = "M.A.R.S.",
		}
	};

	Application app;
	app.Initialize(config);
	app.Run();
	app.Destroy();
}