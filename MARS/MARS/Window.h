#pragma once

#include <Math/Vector.h>

#include <string>

class Window
{
public:
	struct Config
	{
		int32_t     Width;
		int32_t     Height;
		std::string Title;
	};

public:
	void Initialize(const Config& config);
	void Destroy();

	Vector2D GetSize() const;
	inline struct SDL_Window* GetHandle() const { return p_Handle; }

private:
	struct SDL_Window* p_Handle = nullptr;
};