#pragma once

#include <Render/Renderer.h>
#include <Core.h>

#include <string>
#include <vector>
#include <unordered_map>

namespace OpenGL
{
	class Texture;
	class MeshGroup;
}

struct AssetInfo
{
	ID ID;
	std::string Name;
	std::string Path;

	std::string Title() const;
};

class AssetManager // Only GL for now
{
private:
	struct UIData
	{
		struct {
			bool Show = false;
			bool Flip = false;
			char PathInputBuffer[2048];
			int32_t HorizontalWrappingModeIdx = 0;
			int32_t VerticalWrappingModeIdx = 0;
			int32_t FilteringModeIdx = 0;
		} LoadTextureMenu;
		struct {
			bool Show = false;
			char PathInputBuffer[2048];
		} LoadModelMenu;
	};

public:
	void Initialize();

	ID LoadTexture(const std::string& path, bool flip = false, const TextureParams& params = { WrappingMode::Wrap, WrappingMode::Wrap, FilteringMode::Linnear });
	ID LoadMeshes(const std::string& path);

	void DestroyTexture(ID id);
	void DestroyMeshes(ID id);

	OpenGL::Texture* GetTexture(ID id);
	OpenGL::MeshGroup* GetMeshes(ID id);

	std::vector<AssetInfo> MeshesInfo() const;
	std::vector<AssetInfo> TexturesInfo() const;

	void DrawUI();

private:
	static ID GetNextID();

	void DrawTextures();
	void DrawMeshes();

	void DrawLoadTextureMenu();
	void DrawLoadModelMenu();

private:
	std::unordered_map<ID, OpenGL::Texture*>   m_Textures;
	std::unordered_map<ID, OpenGL::MeshGroup*> m_MeshGroups;

	UIData m_UIData;
};

void UpdateTexture(ID& targetID, int32_t& selectedItem, const std::string& title);