#include <Scene/Model.h>
#include <Render/OpenGL/Assets/GL_MeshGroup.h>
#include <Render/OpenGL/GL_Shader.h>
#include <AssetManager/AssetManager.h>

#include <External/imgui/imgui.h>

#include <Global.h>

Model::Model(const std::string& name, const Transform& transform, ID assetID)
	: m_MeshGruopID{assetID}
	, Actor{name, transform}
{
	int32_t lenght = AppAssets->GetMeshes(assetID)->GetLength();
	for (int32_t idx = 0; idx < lenght; ++idx)
	{
		m_SubTransforms.push_back(Transform{});
	}
	m_MeshUB.Initialize(sizeof(Matrix), 0);
	m_MaterialUB.Initialize(sizeof(Material), 1);
}

ActorType Model::GetType() const { return ActorType::Model; }

void Model::Render()
{
	OpenGL::MeshGroup* meshGroup = AppAssets->GetMeshes(m_MeshGruopID);

	for (int32_t idx = 0; idx < meshGroup->GetLength(); ++idx)
	{
		const Transform transform = m_Transform + m_SubTransforms[idx];
		const Matrix modelMatrix = transform.GetModelMatrix();

		m_MeshUB.Update(&modelMatrix);
		m_MeshUB.Bind();

		m_MaterialUB.Update(&m_Material);
		m_MaterialUB.Bind();

		OpenGL::Shader shader;
		shader.Load("MARS/Resources/Shaders/OpenGL/SceneModel.glsl");
		shader.Bind();

		shader.LinkUniformBuffer(AppScene->GetCameraUniform(), "CameraData");
		shader.LinkUniformBuffer(AppScene->GetLightsUniform(), "LightsData");
		shader.LinkUniformBuffer(m_MeshUB, "MeshData");

		OpenGL::Mesh* mesh = meshGroup->GetMesh(idx);
		mesh->Bind();

		GL_CALL(glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), OpenGL::NativeIndexType, nullptr));
		shader.Destroy();
	}
}

void Model::DrawUI()
{
	if (ImGui::TreeNode(m_Name.c_str()))
	{
		ImGui::Begin(m_Name.c_str());

		UpdateTransform(m_Transform);

		OpenGL::MeshGroup* group = AppAssets->GetMeshes(m_MeshGruopID);
		for (uint32_t idx = 0; idx < group->GetLength(); ++idx)
		{
			OpenGL::Mesh* subMesh = group->GetMesh(idx);
			if (ImGui::TreeNode(subMesh->GetName().c_str()))
			{
				UpdateTransform(m_SubTransforms[idx]);
				ImGui::TreePop();
			}
		}

		ImGui::Separator();
		if (ImGui::TreeNode("Material"))
		{
			UpdateColor(m_Material.Diffuse, "Diffuse Color");
			UpdateColor(m_Material.Ambient, "Ambient Color");
			UpdateColor(m_Material.Specular, "Specular Color");
			ImGui::DragFloat("Shininess", &m_Material.Shininess, 0.2f, 0.f, 512.f, "%.3f");
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
