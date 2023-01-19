#include <Scene/Cube.h>

#include <AssetManager/AssetManager.h>

#include <Render/OpenGL/Assets/GL_MeshGroup.h>
#include <Render/OpenGL/Assets/GL_Texture.h>

#include <External/imgui/imgui.h>

#include <Global.h>

void Cube::InitializeResourses()
{
	Cube::s_ModelID = AppAssets->LoadMeshes("MARS/Resources/Meshes/Cube.obj");
	Cube::s_Shader.Load("MARS/Resources/Shaders/OpenGL/CubeTextured.glsl");
}

Cube::Cube(const std::string& name, const Transform& transform) : Actor{ name, transform }
{
	m_MeshUB.Initialize(sizeof(Matrix), 0);
	m_MaterialUB.Initialize(sizeof(float), 1);
}

ActorType Cube::GetType() const
{
	return ActorType::Cube;
}

void Cube::DrawUI()
{
	if (ImGui::TreeNode(m_Name.c_str()))
	{
		ImGui::Begin(m_Name.c_str());

		UpdateTransform(m_Transform);

		if (ImGui::TreeNode("Material"))
		{
			UpdateTexture(m_DiffuseTextureID, m_UIData.DiffuseIndex, "Diffuse");
			UpdateTexture(m_SpecularTextureID, m_UIData.SpecularIndex, "Specular");
			UpdateTexture(m_EmissionTextureID, m_UIData.EmissionIndex, "Emissive");
			ImGui::DragFloat("Shininess", &m_Shininess, 1.f, 1.f, 1000.f);
			ImGui::TreePop();
		}

		ImGui::End();

		for (Actor* child : m_Childs)
		{
			child->DrawUI();
		}
		ImGui::TreePop();
	}
}

void Cube::Render()
{
	const Matrix modelMatrix = m_Transform.GetModelMatrix();

	m_MeshUB.Update(&modelMatrix);
	m_MaterialUB.Update(&m_Shininess);

	const OpenGL::Texture* diffuse = AppAssets->GetTexture(m_DiffuseTextureID);
	const OpenGL::Texture* specular = AppAssets->GetTexture(m_SpecularTextureID);
	const OpenGL::Texture* emission = AppAssets->GetTexture(m_EmissionTextureID);

	diffuse->Bind(0);
	specular->Bind(1);
	emission->Bind(2);

	Cube::s_Shader.Bind();
	Cube::s_Shader.LinkUniformBuffer(AppScene->GetCameraUniform(), "CameraData");
	Cube::s_Shader.LinkUniformBuffer(AppScene->GetLightsUniform(), "LightsData");
	Cube::s_Shader.LinkUniformBuffer(AppScene->GetDirLightUniform(), "DirectionalLightData");
	Cube::s_Shader.LinkUniformBuffer(m_MeshUB, "MeshData");
	Cube::s_Shader.LinkUniformBuffer(m_MaterialUB, "MaterialData");
	Cube::s_Shader.LinkTexture("DiffuseTexture", 0);
	Cube::s_Shader.LinkTexture("SpecularTexture", 1);
	Cube::s_Shader.LinkTexture("EmissionTexture", 2);

	const OpenGL::Mesh* mesh = AppAssets->GetMeshes(Cube::s_ModelID)->GetMesh(0);
	mesh->Bind();

	GL_CALL(glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), OpenGL::NativeIndexType, nullptr));
	printf("Draw: %s\n", m_Name.c_str());
}