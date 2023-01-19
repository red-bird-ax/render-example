#include <Scene/Scene.h>
#include <Scene/Model.h>
#include <Scene/Camera.h>
#include <Scene/PointLight.h>
#include <Scene/DirectionalLight.h>
#include <Scene/Cube.h>

#include <AssetManager/AssetManager.h>

#include <External/imgui/imgui.h>

#include <Global.h>

#include <sstream>

void Scene::DrawUI()
{
	DrawActorTree();
	DrawAddActorMenu();
}

void Scene::DrawActorTree()
{
	ImGui::Begin("Scene:");

	if (ImGui::Button("Add Actor"))
	{
		m_UIData.AddActorMenu.Show = true;
	}

	for (Actor* actor : m_Root)
	{
		actor->DrawUI();
	}

	ImGui::End();
}

void Scene::DrawAddActorMenu()
{
	if (m_UIData.AddActorMenu.Show)
	{
		ImGui::Begin("Add Actor:", &m_UIData.AddActorMenu.Show);

		static const char* actorTypes[] = {
			"Model", "Camera", "Point light", "Directional light", "Spotlight", "Cube",
		};
		static const int32_t typesCount = IM_ARRAYSIZE(actorTypes);

		ImGui::Combo("Actor Type", &m_UIData.AddActorMenu.SelectedItemIdx, actorTypes, typesCount);

		const ActorType type = static_cast<ActorType>(m_UIData.AddActorMenu.SelectedItemIdx);
		switch (type)
		{
			case ActorType::Model:
			{
				DrawAddModel();
				break;
			}

			case ActorType::Camera:
			{
				DrawAddCamera();
				break;
			}

			case ActorType::PointLight:
			{
				DrawAddPointLight();
				break;
			}

			case ActorType::DirectionalLight:
			{
				DrawAddDirectionalLight();
				break;
			}

			case ActorType::Cube:
			{
				DrawAddCube();
				break;
			}

			default: break;
		}
		ImGui::End();
	}
}

void Scene::DrawAddModel()
{
	auto& uiData = m_UIData.AddActorMenu.Model;
	const std::vector<AssetInfo> meshes = AppAssets->MeshesInfo();
	if (meshes.size() > 0)
	{
		// Transform:
		ImGui::Separator();
		UpdateTransform(uiData.Transform);
		ImGui::Separator();

		// Asset:
		const AssetInfo selectedModel = meshes[uiData.SelectdModelIdx];
		if (ImGui::BeginCombo("Asset", selectedModel.Title().c_str()))
		{
			for (int32_t idx = 0; idx < meshes.size(); ++idx)
			{
				bool selected;
				const AssetInfo model = meshes[idx];
				if (ImGui::Selectable(model.Title().c_str(), &selected))
				{
					uiData.SelectdModelIdx = idx;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();

		// Name:
		UpdateString(uiData.Name, "Name");

		// Material:
		if (ImGui::TreeNode("Material"))
		{
			UpdateColor(uiData.Diffuse, "Diffuse Color");
			UpdateColor(uiData.Ambient, "Ambient Color");
			UpdateColor(uiData.Specular, "Specular Color");
			ImGui::DragFloat("Shininess", &uiData.Shininess, 0.2f, 0.f, 512.f, "%.3f");
			ImGui::TreePop();
		}
		ImGui::Separator();

		// Button:
		if (ImGui::Button("Create"))
		{
			std::string name = uiData.Name == ""
				? selectedModel.Name
				: uiData.Name;

			Model* model = new Model{ name, uiData.Transform, selectedModel.ID };
			model->SetMaterial(Material{
				.Ambient   = uiData.Ambient,
				.Diffuse   = uiData.Diffuse,
				.Specular  = uiData.Specular,
				.Shininess = uiData.Shininess,
			});

			m_Root.push_back(model);
			m_UIData.AddActorMenu.Show = false;
		}
	}
	// No models loaded:
	else
	{
		ImGui::Text("There are no loaded models...");
	}
}

void Scene::DrawAddCube()
{
	auto& uiData = m_UIData.AddActorMenu.Cube;

	UpdateString(uiData.Name, "Name");
	UpdateTransform(uiData.Transform);
	UpdateTexture(uiData.DiffuseID, uiData.DiffuseIndex, "Diffuse");
	UpdateTexture(uiData.SpecularID, uiData.SpecularIndex, "Specular");
	UpdateTexture(uiData.EmissiveID, uiData.EmissiveIndex, "Emissive");

	if (ImGui::Button("Create"))
	{
		const std::string name = uiData.Name == "" ? "Cube" : uiData.Name;

		Cube* cube = new Cube{ name, uiData.Transform };
		cube->SetTextures(uiData.DiffuseID, uiData.SpecularID, uiData.EmissiveID);

		m_Root.push_back(cube);
		m_UIData.AddActorMenu.Show = false;
	}
}

void Scene::DrawAddCamera()
{
	auto& uiData = m_UIData.AddActorMenu.Camera;

	ImGui::Separator();
	UpdateTransform(uiData.Transform);
	UpdateString(uiData.Name, "Name");

	static const char* projections[] = { "Perspective", "Orthographic" };
	ImGui::Combo("Projection", &uiData.ProjectionIdx, projections, IM_ARRAYSIZE(projections));
	ImGui::Separator();

	const CameraType type = static_cast<CameraType>(uiData.ProjectionIdx);
	switch (type)
	{
		case CameraType::Perspective:
		{
			ImGui::SliderFloat("FOV", &uiData.FOV, 10.f, 160.f);
			ImGui::SliderFloat("Near Plane", &uiData.Near, 0.0001f, 0.1f);
			ImGui::SliderFloat("Far Plane", &uiData.Far, 1.f, 10'000.f);
			break;
		}

		case CameraType::Orthographic:
		{
			ImGui::SliderFloat("Near Plane", &uiData.Near, 0.0001f, 0.1f);
			ImGui::SliderFloat("Far Plane", &uiData.Far, 1.f, 10'000.f);
			break;
		}
	}

	if (ImGui::Button("Create"))
	{
		std::string name = uiData.Name == ""
			? "Camera"
			: uiData.Name;

		const Vector2D windowSize = AppWindow->GetSize();
		const float aspectRatio = windowSize.X / windowSize.Y;

		Camera* camera = new Camera{ name, uiData.Transform };
		switch (type)
		{
			case CameraType::Perspective:
			{
				camera->BuildPerspective(uiData.FOV, aspectRatio, uiData.Near, uiData.Far);
				break;
			}

			case CameraType::Orthographic:
			{
				camera->BuildOrthographic(aspectRatio, uiData.Near, uiData.Far);
				break;
			}
		}

		m_Root.push_back(camera);
		m_UIData.AddActorMenu.Show = false;
	}
}

void Scene::DrawAddPointLight()
{
	auto& uiData = m_UIData.AddActorMenu.PointLight;

	ImGui::Separator();
	UpdateTransform(uiData.Transform);
	UpdateString(uiData.Name, "Name");

	ImGui::DragFloat("Light Intensity", &uiData.Intensity, 0.5f, 0.f, 10'000.f);

	if (ImGui::TreeNode("Parameters"))
	{
		UpdateColor(uiData.Diffuse, "Diffuse Color");
		UpdateColor(uiData.Ambient, "Ambient Color");
		UpdateColor(uiData.Specular, "Specular Color");
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::Button("Create"))
	{
		if (uiData.Name == "")
		{
			uiData.Name = "Point light";
		}

		PointLight* light = new PointLight{ uiData.Name, uiData.Transform };
		light->SetProperties({
			.Ambient  = uiData.Ambient,
			.Diffuse  = uiData.Diffuse,
			.Specular = uiData.Specular,
		});

		m_Root.push_back(light);
		m_UIData.AddActorMenu.Show = false;
	}
}

void Scene::DrawAddDirectionalLight()
{
	auto& uiData = m_UIData.AddActorMenu.DirLight;

	ImGui::Separator();
	UpdateTransform(uiData.Transform);
	UpdateString(uiData.Name, "Name");

	if (ImGui::TreeNode("Parameters"))
	{
		UpdateColor(uiData.Diffuse, "Diffuse Color");
		UpdateColor(uiData.Ambient, "Ambient Color");
		UpdateColor(uiData.Specular, "Specular Color");
		ImGui::TreePop();
	}
	ImGui::Separator();

	if (ImGui::Button("Create"))
	{
		if (uiData.Name == "")
		{
			uiData.Name = "Directional light";
		}

		DirectionalLight* light = new DirectionalLight{ uiData.Name, uiData.Transform };
		light->SetProperties({
			.Ambient  = uiData.Ambient,
			.Diffuse  = uiData.Diffuse,
			.Specular = uiData.Specular,
		});

		m_Root.push_back(light);
		m_UIData.AddActorMenu.Show = false;
	}
}

void UpdateTransform(Transform& transform)
{
	float location[3] = { transform.Location.X, transform.Location.Y, transform.Location.Z };
	float rotation[3] = { transform.Rotation.Roll, transform.Rotation.Pitch, transform.Rotation.Yaw };
	float scale[3]    = { transform.Scale.X, transform.Scale.Y, transform.Scale.Z };

	ImGui::Text("Transform:");
	ImGui::DragFloat3("Location", location, 0.1f, -100'000, 100'000, "%.3f");
	ImGui::DragFloat3("Rotation", rotation, 1.f, -360, 360, "%.3f");
	ImGui::DragFloat3("Scale", scale, 0.1f, 0, 1'000, "%.3f");
	ImGui::Separator();

	transform.Location = { location[0], location[1], location[2] };
	transform.Rotation = { rotation[0], rotation[1], rotation[2] };
	transform.Scale    = { scale[0], scale[1], scale[2] };
}


void UpdateColor(NormColor& color, const std::string& title)
{
	float rgb[3] = { color.R, color.G, color.B };
	ImGui::ColorPicker3(title.c_str(), rgb);
	ImGui::Separator();

	color.R = rgb[0];
	color.G = rgb[1];
	color.B = rgb[2];
}

void UpdateString(std::string& string, const std::string& title)
{
	char buffer[2048];
	memcpy(buffer, string.c_str(), string.size());
	buffer[string.size()] = '\0';

	ImGui::InputText(title.c_str(), buffer, IM_ARRAYSIZE(buffer));
	ImGui::Separator();

	string = buffer;
}