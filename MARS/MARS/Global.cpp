#include <Global.h>
#include <Application.h>
#include <AssetManager/AssetManager.h>
#include <Scene/Scene.h>

void Global::Initialize(Application* app)
{
	p_Application = app;
}

Window* Global::GetWindow()
{
	return &p_Application->m_Window;
}

IRenderer* Global::GetRenderer()
{
	return p_Application->p_Renderer;
}

AssetManager* Global::GetAssetManager()
{
	return &p_Application->m_Assets;
}

Scene* Global::GetScene()
{
	return &p_Application->m_Scene;
}