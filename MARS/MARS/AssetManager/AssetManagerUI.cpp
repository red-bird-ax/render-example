#include <AssetManager/AssetManager.h>

#include <Render/OpenGL/Assets/GL_MeshGroup.h>
#include <Render/OpenGL/Assets/GL_Texture.h>

#include <External/imgui/imgui.h>

#include <Global.h>

void AssetManager::DrawUI()
{
	ImGui::Begin("Asset Manager:");

	if (ImGui::TreeNode("Textures:"))
	{
		DrawTextures();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Models:"))
	{
		DrawMeshes();
		ImGui::TreePop();
	}

	ImGui::End();

	DrawLoadTextureMenu();
	DrawLoadModelMenu();
}

void AssetManager::DrawTextures()
{
	ImGui::Text("Textures loaded: %d", m_Textures.size());
	if (ImGui::Button("Load texture"))
	{
		m_UIData.LoadTextureMenu.Show = true;
	}
	ImGui::Separator();

	for (const auto [id, texture] : m_Textures)
	{
		if (ImGui::TreeNode(texture->GetName().c_str()))
		{
			ImGui::Text("Asset ID: %d", id);
			ImGui::Text("Path: %s", texture->GetPath().c_str());

			const Vector2D size = texture->GetSize();
			ImGui::Text("Width: %f, Height: %f", size.X, size.Y);
			ImGui::Text("Channels number: %d", texture->GetChannelsNumber());
			ImGui::Image((void*)(intptr_t)texture->GetHandle(), ImVec2(450, 450));

			ImGui::TreePop();
		}
		ImGui::Separator();
	}
}

void AssetManager::DrawMeshes()
{
	ImGui::Text("Models laoded: %d", m_MeshGroups.size());
	if (ImGui::Button("Load model"))
	{
		m_UIData.LoadModelMenu.Show = true;
	}
	ImGui::Separator();

	for (const auto [id, meshes] : m_MeshGroups)
	{
		if (ImGui::TreeNode(meshes->GetName().c_str()))
		{
			ImGui::Text("Asset ID: %d", id);
			ImGui::Text("Path: %s", meshes->GetPath().c_str());
			ImGui::Text("Total vertices: %d", meshes->GetToltalVertices());
			ImGui::Text("Total faces: %d", meshes->GetToltalFaces());

			ImGui::Text("Meshes:");
			for (const auto& mesh : *meshes)
			{
				if (ImGui::TreeNode(mesh.GetName().c_str()))
				{
					ImGui::Text("Vertices count: %d", mesh.GetVerticesCount());
					ImGui::Text("Indices count: %d", mesh.GetIndicesCount());
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::Separator();
	}
}

void AssetManager::DrawLoadTextureMenu()
{
	if (m_UIData.LoadTextureMenu.Show)
	{
		ImGui::Begin("Load Texture", &m_UIData.LoadTextureMenu.Show);
		
		ImGui::InputText("File Path", m_UIData.LoadTextureMenu.PathInputBuffer, IM_ARRAYSIZE(m_UIData.LoadTextureMenu.PathInputBuffer));
		ImGui::Separator();

		static const char* filteringModes[] = { "Linnear", "Point" };
		static const char* wrappingModes[]  = { "Wrap", "Mirror", "Clamp", "Border" };

		ImGui::Combo("Horizontal Wrapping", &m_UIData.LoadTextureMenu.HorizontalWrappingModeIdx, wrappingModes, IM_ARRAYSIZE(wrappingModes));
		ImGui::Combo("Vertical Wrapping", &m_UIData.LoadTextureMenu.VerticalWrappingModeIdx, wrappingModes, IM_ARRAYSIZE(wrappingModes));
		ImGui::Combo("Filtering Mode", &m_UIData.LoadTextureMenu.FilteringModeIdx, filteringModes, IM_ARRAYSIZE(filteringModes));
		ImGui::Separator();

		ImGui::Checkbox("Flip", &m_UIData.LoadTextureMenu.Flip);
		ImGui::Separator();

		if (ImGui::Button("Load"))
		{
			const TextureParams params
			{
				.HorizontalWrapping = static_cast<WrappingMode>(m_UIData.LoadTextureMenu.HorizontalWrappingModeIdx),
				.VerticalWrapping   = static_cast<WrappingMode>(m_UIData.LoadTextureMenu.VerticalWrappingModeIdx),
				.Filtering          = static_cast<FilteringMode>(m_UIData.LoadTextureMenu.FilteringModeIdx),
			};
			LoadTexture(m_UIData.LoadTextureMenu.PathInputBuffer, m_UIData.LoadTextureMenu.Flip, params);
			m_UIData.LoadTextureMenu.Show = false;
		}

		ImGui::End();
	}
}

void AssetManager::DrawLoadModelMenu()
{
	if (m_UIData.LoadModelMenu.Show)
	{
		ImGui::Begin("Load Model", &m_UIData.LoadModelMenu.Show);

		ImGui::InputText("File Path", m_UIData.LoadModelMenu.PathInputBuffer, IM_ARRAYSIZE(m_UIData.LoadModelMenu.PathInputBuffer));
		ImGui::Separator();

		ImGui::Text("TODO: add aiProcess_ flags options");
		ImGui::Separator();

		if (ImGui::Button("Load"))
		{
			LoadMeshes(m_UIData.LoadModelMenu.PathInputBuffer);
			m_UIData.LoadModelMenu.Show = false;
		}

		ImGui::End();
	}
}

void UpdateTexture(ID& targetID, int32_t& selectedItem, const std::string& title)
{
	if (ImGui::TreeNode(title.c_str()))
	{
		const std::vector<AssetInfo> textures = AppAssets->TexturesInfo();
		const AssetInfo selectedTexture = textures[selectedItem];

		if (targetID != 0)
		{
			for (int32_t idx = 0; idx < textures.size(); ++idx)
			{
				if (textures[idx].ID == targetID)
				{
					selectedItem = idx;
					break;
				}
			}
		}

		if (ImGui::BeginCombo("Asset", selectedTexture.Title().c_str()))
		{
			for (int32_t idx = 0; idx < textures.size(); ++idx)
			{
				bool selected;
				const AssetInfo& texture = textures[idx];
				if (ImGui::Selectable(texture.Title().c_str(), &selected))
				{
					selectedItem = idx;
					targetID = texture.ID;
				}
			}
			ImGui::EndCombo();
		}

		const OpenGL::Texture* texture = AppAssets->GetTexture(selectedTexture.ID);

		ImGui::Image((void*)(intptr_t)texture->GetHandle(), ImVec2(320, 320));
		ImGui::Separator();
		ImGui::TreePop();
	}
}