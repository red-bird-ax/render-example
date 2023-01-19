#include <Application.h>
#include <Global.h>

#include <Math/Color.h>

#include <Scene/Cube.h>
#include <Scene/Camera.h>
#include <Scene/PointLight.h>

void Application::Initialize(const Config& config)
{
	const Window::Config windowConfig
	{
		.Width  = config.Window.Width,
		.Height = config.Window.Height,
		.Title  = config.Window.Title,
	};
	m_Window.Initialize(windowConfig);

	p_Renderer = CreateRenderer();
	p_Renderer->Initialize(m_Window);

	m_Assets.Initialize();
	m_Scene.Initialize();
	m_LuaVM.Intitalize();

	Global::Initialize(this);


	// for test:
	AppAssets->LoadTexture("MARS/Resources/Textures/T_Cube_S.png");
	AppAssets->LoadTexture("MARS/Resources/Textures/T_Cube_D.png");
	AppAssets->LoadTexture("MARS/Resources/Textures/T_Cube_E.png");
	
	AppAssets->LoadMeshes("MARS/Resources/Meshes/Male.obj");
	AppAssets->LoadMeshes("MARS/Resources/Meshes/Nanosuit.obj");
	
	Cube::InitializeResourses();
	
}

void Application::Destroy()
{
	m_LuaVM.Destroy();
	m_Window.Destroy();
}

void Application::Run()
{
	Camera* cam = new Camera{ "Camera", Transform{} };
	cam->BuildPerspective(45.f, 16.f / 9.f, 0.001f, 1000.f);

	PointLight* pl = new PointLight{ "Point Light", Transform{} };
	pl->SetProperties(LightProperties{
		.Ambient  = { 0.1f, 0.1f, 0.1f, 1.f },
		.Diffuse  = { 0.9f, 0.9f, 0.9f, 1.f },
		.Specular = { 1.f, 1.f, 1.f, 1.f },
	});

	AppScene->AddChild(cam);
	AppScene->AddChild(pl);


	m_Running = true;

	while (m_Running)
	{
		HandleEvents();

		p_Renderer->BeginFrame({ 40, 50, 60, 255 });
		//p_Renderer->Test01();
		m_Scene.Render();
		m_Scene.DrawUI();
		m_Assets.DrawUI();
		p_Renderer->EndFrame();
	}
}