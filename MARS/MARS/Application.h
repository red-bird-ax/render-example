#pragma once

#include <Events.h>
#include <Render/Renderer.h>
#include <AssetManager/AssetManager.h>
#include <Scripting/LuaVM.h>
#include <Scene/Scene.h>

class Application
{
public:
	struct Config
	{
		struct
		{
			int32_t Width  = 1920;
			int32_t Height = 1080;

			std::string Title;
		} Window;
	};

public:
	void Initialize(const Config& config);
	void Destroy();

	void Run();

private:
	void HandleEvents();
	void OnQuitEvent();
	void OnMouseMotionEvent(MouseMotionEvent& event);
	void OnMouseButtonEvent(MouseButtonEvent& event);
	void OnKeyboardEvent(KeyboardEvent& event);

private:
	Window m_Window;
	AssetManager m_Assets;
	Scene m_Scene;
	IRenderer* p_Renderer = nullptr;
	LuaVM m_LuaVM;

	bool m_Running = false;
	
private:
	friend class Global;
};