#pragma once

#include <Render/Renderer.h>

#define AppWindow   Global::GetWindow()
#define AppRenderer Global::GetRenderer()
#define AppAssets   Global::GetAssetManager()
#define AppScene    Global::GetScene()

class Application;
class Window;
//class IRenderer;
class AssetManager;
class Scene;

class Global
{
public:
	static void Initialize(Application* app);

	static Window* GetWindow();
	static IRenderer* GetRenderer();
	static AssetManager* GetAssetManager();
	static Scene* GetScene();

private:
	inline static Application* p_Application = nullptr;
};