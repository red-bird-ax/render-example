#include <AssetManager/AssetManager.h>
#include <Render/OpenGL/Assets/GL_Texture.h>
#include <Render/OpenGL/Assets/GL_MeshGroup.h>

#include <Utility/FileUtility.h>

#include <External/stb/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <sstream>

void AssetManager::Initialize()
{
	
}

ID AssetManager::LoadTexture(const std::string& path, bool flip, const TextureParams& params)
{
	stbi_set_flip_vertically_on_load(flip);

	// Read texture:
	int32_t width, height, channels;
	uint8_t* buffer = stbi_load(path.c_str(), &width, &height, &channels, 4);
	ASSERT(buffer != nullptr, "Failed to read texture");

	// Save texture:
	OpenGL::Texture* texture = new OpenGL::Texture;
	texture->Initialize(buffer, width, height, channels, params);
	texture->SetPath(path);
	texture->SetName(ClipFileName(path));

	const ID id = AssetManager::GetNextID();
	//m_Textures[id] = texture;
	m_Textures.emplace(id, texture);

	// Free texture:
	stbi_image_free(buffer);
	return id;
}

void AssetManager::DestroyTexture(ID id)
{
	if (m_Textures.contains(id))
	{
		OpenGL::Texture* texture = m_Textures.at(id);

		texture->Destroy();
		delete texture;

		m_Textures.erase(id);
	}
}

OpenGL::Texture* AssetManager::GetTexture(ID id)
{
	if (m_Textures.contains(id))
	{
		return m_Textures.at(id);
	}
	return nullptr;
}

ID AssetManager::LoadMeshes(const std::string& path)
{
	struct Vertex
	{
		Vector3D Location;
		Vector3D Normal;
		Vector2D UV;
	};

	// Read model:
	Assimp::Importer importer;
	const aiScene* impModel = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	ASSERT(impModel != nullptr, (std::string{ "Failed to read model: " } + importer.GetErrorString()).c_str());

	// Meshes allocation:
	const uint32_t meshesCount = impModel->mNumMeshes;
	OpenGL::Mesh* meshes = new OpenGL::Mesh[meshesCount];
	for (uint32_t meshIdx = 0; meshIdx < meshesCount; ++meshIdx)
	{
		const aiMesh* impMesh = impModel->mMeshes[meshIdx];

		// Vertices allocation:
		const uint32_t verticesCount = impMesh->mNumVertices;
		Vertex* vertices = new Vertex[verticesCount];

		for (uint32_t vertexIdx = 0; vertexIdx < verticesCount; ++vertexIdx)
		{
			if (impMesh->HasPositions())
			{
				const auto [vx, vy, vz] = impMesh->mVertices[vertexIdx];
				vertices[vertexIdx].Location = { vx, vy, vz };
			}
			if (impMesh->HasNormals())
			{
				const auto [nx, ny, nz] = impMesh->mNormals[vertexIdx];
				vertices[vertexIdx].Normal = { nx, ny, nz };
			}
			if (impMesh->HasTextureCoords(0))
			{
				const auto [u, v, unused] = impMesh->mTextureCoords[0][vertexIdx]; // TODO: do smth about slot[0]
				vertices[vertexIdx].UV = { u, v };
			}
		}

		// Indices allocation:
		const uint32_t indicesCount = impMesh->mNumFaces * 3;
		OpenGL::IndexType* indices = new OpenGL::IndexType[indicesCount];
		uint32_t indexID = 0;

		for (uint32_t faceIdx = 0; faceIdx < impMesh->mNumFaces; ++faceIdx)
		{
			const aiFace face = impMesh->mFaces[faceIdx];
			for (uint32_t idx = 0; idx < face.mNumIndices; ++idx)
			{
				indices[indexID] = face.mIndices[idx];
				indexID++;
			}
		}

		// Mesh initialization:
		OpenGL::VertexBuffer vb;
		vb.Initialize(verticesCount, vertices);

		OpenGL::IndexBuffer ib;
		ib.Initialize(indicesCount, indices);

		std::string meshName = impMesh->mName.C_Str();
		if (meshName == "")
		{
			meshName = "Mesh #";
			meshName += meshIdx;
		}

		meshes[meshIdx].BuildFromBuffers(vb, ib, OpenGL::VertexLayoutNT);
		meshes[meshIdx].SetName(meshName);

		// Free memory:
		delete[] vertices;
		delete[] indices;
	}

	// Save meshes:
	std::string modelName = impModel->mName.C_Str();
	if (modelName == "")
	{
		modelName = ClipFileName(path);
	}

	OpenGL::MeshGroup* group = new OpenGL::MeshGroup;
	group->Initialize(modelName, meshesCount, meshes);
	group->SetPath(path);

	const ID id = AssetManager::GetNextID();
	m_MeshGroups.emplace(id, group);

	return id;
}

void AssetManager::DestroyMeshes(ID id)
{
	if (m_MeshGroups.contains(id))
	{
		OpenGL::MeshGroup* meshes = m_MeshGroups.at(id);

		meshes->Destroy();
		delete meshes;

		m_MeshGroups.erase(id);
	}
}

OpenGL::MeshGroup* AssetManager::GetMeshes(ID id)
{
	if (m_MeshGroups.contains(id))
	{
		return m_MeshGroups.at(id);
	}
	return nullptr;
}

std::vector<AssetInfo> AssetManager::MeshesInfo() const
{
	std::vector<AssetInfo> info;
	for (const auto [id, meshes] : m_MeshGroups)
	{
		info.emplace_back(id, meshes->GetName(), meshes->GetPath());
	}
	return info;
}

std::vector<AssetInfo> AssetManager::TexturesInfo() const
{
	std::vector<AssetInfo> info;
	for (const auto [id, texture] : m_Textures)
	{
		info.emplace_back(id, texture->GetName(), texture->GetPath());
	}
	return info;
}

ID AssetManager::GetNextID()
{
	return rand();
}

std::string AssetInfo::Title() const
{
	std::stringstream stream;
	stream << Name << " #" << ID;
	return stream.str();
}