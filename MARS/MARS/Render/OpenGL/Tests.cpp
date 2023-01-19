#include <Render/OpenGL/GL_Renderer.h>

#include <Render/OpenGL/Buffers/GL_IndexBuffer.h>
#include <Render/OpenGL/Buffers/GL_UniformBuffer.h>
#include <Render/OpenGL/Buffers/GL_VertexBuffer.h>
#include <Render/OpenGL/Buffers/GL_VertexLayout.h>

#include <Render/OpenGL/Assets/GL_MeshGroup.h>
#include <Render/OpenGL/Assets/GL_Texture.h>

#include <Render/OpenGL/Scene/GL_Camera.h>
#include <Render/OpenGL/Scene/GL_Model.h>

#include <Render/OpenGL/GL_Shader.h>

#include <External/imgui/imgui.h>

#include <AssetManager/AssetManager.h>
#include <Global.h>

namespace OpenGL
{
	// Scene objects, ImGUI (docking)
	void Renderer::Test01()
	{
		// Data:
		struct Vertex
		{
			struct { float X, Y, Z; } Location;
			struct { float R, G, B; } Color;
			struct { float U, V; }    TexCoords;
		};

		const Vertex vertices[4] = {
			// positions               // colors              // texture coords
			{ {  0.5f,  0.5f, 0.0f },  { 1.0f, 0.0f, 0.0f },  { 1.0f, 1.0f }  },
			{ {  0.5f, -0.5f, 0.0f },  { 0.0f, 1.0f, 0.0f },  { 1.0f, 0.0f }, },
			{ { -0.5f, -0.5f, 0.0f },  { 0.0f, 0.0f, 1.0f },  { 0.0f, 0.0f }, },
			{ { -0.5f,  0.5f, 0.0f },  { 1.0f, 1.0f, 0.0f },  { 0.0f, 1.0f }, },
		};

		const IndexType indices[6] = {
			0, 1, 2,
			2, 3, 0,
		};

		struct CameraData
		{
			Matrix MVP;
			Matrix View;
		};

		static float angle = 0.f;
		angle += 0.8f;

		Camera camera;
		camera.BuildPerspective(45.f, 16.f / 9.f, 0.0001, 1000.f);
		camera.SetLocation({ 0.f, 0.f, -5.f });

		Transform transform
		{
			.Location{ 0.f, 0.f, 0.f },
			.Rotation{ 0.f, angle, angle },
			.Scale{ 0.1f, 0.1f, 0.1f  },
		};

		CameraData cameraData
		{
			.MVP = camera.GetViewProjection() * transform.GetModelMatrix(),
			.View = camera.GetView(),
		};

		const ID group = AppAssets->LoadMeshes("MARS/Resources/Meshes/Male.obj");
		

		UniformBuffer ub;
		ub.Initialize(sizeof(cameraData));

		Shader shader;
		shader.Load("MARS/Resources/Shaders/OpenGL/Simpe.glsl");

		const ID tex1 = AppAssets->LoadTexture("MARS/Resources/Textures/Quad NCD.png");
		const ID tex2 = AppAssets->LoadTexture("MARS/Resources/Textures/sign.png");

		shader.Bind();
		auto meshes = AppAssets->GetMeshes(group);
		meshes->GetMesh(0)->Bind();

		shader.LinkTexture("texSampler", 0);

		shader.LinkTexture("texSampler2", 1);
		AppAssets->GetTexture(tex2)->Bind(0);

		shader.LinkUniformBuffer(ub, "CameraData");
		ub.Update(&cameraData);
		ub.Bind();

		GL_CALL(glDrawElements(GL_TRIANGLES, meshes->GetMesh(0)->GetIndicesCount(), NativeIndexType, nullptr));

		ub.Destroy();
		shader.Destroy();
		AppAssets->DestroyMeshes(group);

		AppAssets->DestroyTexture(tex1);
		AppAssets->DestroyTexture(tex2);

		ImGui::ShowDemoWindow(nullptr);
	}
}