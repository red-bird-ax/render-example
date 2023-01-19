#include <Application.h>
#include <External/imgui/imgui_impl_sdl.h>

#include <SDL.h>

MouseButton MapButton(uint8_t index)
{
	switch (index)
	{
	case SDL_BUTTON_LEFT:
	{
		return MouseButton::Left;
	}

	case SDL_BUTTON_RIGHT:
	{
		return MouseButton::Right;
	}

	case SDL_BUTTON_MIDDLE:
	{
		return MouseButton::Middle;
	}

	default: return MouseButton::None;
	}
}

void Application::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type)
		{
			case SDL_QUIT:
			{
				OnQuitEvent();
				break;
			}

			case SDL_MOUSEMOTION:
			{
				MouseMotionEvent mme
				{
					.Position
					{
						.X{ static_cast<float>(event.motion.x)},
						.Y{ static_cast<float>(event.motion.y)},
					},
					.Relative
					{
						.X{ static_cast<float>(event.motion.xrel) },
						.Y{ static_cast<float>(event.motion.yrel) },
					},
				};

				OnMouseMotionEvent(mme);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				int32_t x, y;
				SDL_GetMouseState(&x, &y);

				MouseButtonEvent mbe
				{
					.Position
					{
						.X{ static_cast<float>(x)},
						.Y{ static_cast<float>(y)},
					},
					.Button{ MapButton(event.button.button) },
					.Action = InputAction::Press,
				};

				OnMouseButtonEvent(mbe);
				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				int32_t x, y;
				SDL_GetMouseState(&x, &y);

				MouseButtonEvent mbe
				{
					.Position
					{
						.X{ static_cast<float>(x)},
						.Y{ static_cast<float>(y)},
					},
					.Button{ MapButton(event.button.button) },
					.Action = InputAction::Release,
				};

				OnMouseButtonEvent(mbe);
				break;
			}

			case SDL_KEYDOWN:
			{
				KeyboardEvent ke
				{
					.Key{ static_cast<Key>(event.key.keysym.sym) },
					.Action{ InputAction::Press },
				};

				OnKeyboardEvent(ke);
				break;
			}

			case SDL_KEYUP:
			{
				KeyboardEvent ke
				{
					.Key{ static_cast<Key>(event.key.keysym.sym) },
					.Action{ InputAction::Release },
				};

				OnKeyboardEvent(ke);
				break;
			}

			default: return;
		}
	}
}

void Application::OnQuitEvent()
{
	m_Running = false;
}

void Application::OnMouseMotionEvent(MouseMotionEvent& event)
{

}

void Application::OnMouseButtonEvent(MouseButtonEvent& event)
{

}

void Application::OnKeyboardEvent(KeyboardEvent& event)
{

}