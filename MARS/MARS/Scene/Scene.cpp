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


Scene::Scene()
{

}

void Scene::Initialize()
{
	m_CameraUB.Initialize(sizeof(m_CameraData), 8);
	m_LightsUB.Initialize(sizeof(m_LightsData), 9);
	m_DirectionalLightBuffer.Initialize(sizeof(m_DirectionalLightPayload), 10);
}

Camera* Scene::GetActiveCamera()
{
	for (Actor* actor : m_Root) // Bug! Not recursice call
	{
		if (actor->GetType() == ActorType::Camera)
		{
			Camera* camera = dynamic_cast<Camera*>(actor);
			if (camera->IsActive())
			{
				return camera;
			}
		}
	}
	return nullptr;
}

void Scene::UpdateCamera(Camera* camera)
{
	m_CameraData.ViewProjection = camera->GetViewProjection();
	m_CameraData.View = camera->GetView();
	m_CameraData.CameraLocation = camera->GetTransform().Location;

	m_CameraUB.Update(&m_CameraData);
	m_CameraUB.Bind();
}

std::vector<Model*>* Scene::GetActiveModels()
{
	static std::vector<Model*> models;
	models.clear();

	for (Actor* actor : m_Root)
	{
		if (actor->GetType() == ActorType::Model)
		{
			// TODO: impl isVisivle check
			models.push_back(dynamic_cast<Model*>(actor));
		}
	}

	return &models;
}

std::vector<PointLight*>* Scene::GetActiveLights()
{
	static std::vector<PointLight*> lights;
	lights.clear();

	for (Actor* actor : m_Root)
	{
		if (actor->GetType() == ActorType::PointLight)
		{
			lights.push_back(dynamic_cast<PointLight*>(actor));
		}
	}

	return &lights;
}

void Scene::UpdateLights(std::vector<PointLight*>* lights)
{
	PointLight* light = lights->at(0);

	const auto [ambient, diffuse, specular] = light->GetProperties();
	m_LightsData = {
		.Ambient  = ambient,
		.Diffuse  = diffuse,
		.Specular = specular,
		.Location = light->GetTransform().Location,
	};

	m_LightsUB.Update(&m_LightsData);
	m_LightsUB.Bind();
}

void Scene::Render()
{
	// Camera:
	if (Camera* activeCamera = GetActiveCamera(); activeCamera != nullptr)
	{
		UpdateCamera(activeCamera);
	} else return;


	// Light sources:
	DirectionalLight* directionalLight = nullptr;
	for (auto& actor : m_Root)
	{
		if (actor->GetType() == ActorType::DirectionalLight)
		{
			directionalLight = dynamic_cast<DirectionalLight*>(actor);
		}
	}
	if (directionalLight != nullptr)
	{
		const auto [ambient, diffuse, specular] = directionalLight->GetProperties();
		m_DirectionalLightPayload = {
			.Ambient   = ambient,
			.Diffuse   = diffuse,
			.Specular  = specular,
			.Direction = directionalLight->GetDirection()
		};

		m_DirectionalLightBuffer.Update(&m_DirectionalLightPayload);
		m_DirectionalLightBuffer.Bind();
	} else return;


	if (auto lights = GetActiveLights(); lights->size() > 0)
	{
		UpdateLights(lights);
	} else return;

	// Models:
	/*if (auto models = GetActiveModels(); models->size() > 0)
	{
		for (Model* model : *models)
		{
			model->Render();
		}
	} else return;*/

	for (auto& actor : m_Root) // FIX: now only one entity get`s renderer!
	{
		if (actor->GetType() == ActorType::Cube)
		{
			dynamic_cast<Cube*>(actor)->Render();
		}
	}
}



void Scene::DeactivateCameras()
{
	for (Actor* actor : m_Root)
	{
		if (actor->GetType() == ActorType::Camera)
		{
			dynamic_cast<Camera*>(actor)->SetActive(false);
		}
		for (Actor* child : actor->GetChilds())
		{
			// Bug here! It is not recursive call (only one layer)
			if (child->GetType() == ActorType::Camera)
			{
				dynamic_cast<Camera*>(child)->SetActive(false);
			}
		}
	}
}

Actor::Actor(const std::string& name, const Transform& transform)
	: m_Name{ name }
	, m_Transform{ transform }
{ }

ActorType Actor::GetType() const
{
	return ActorType::Model; // Make pure virtual
}

void Actor::DrawUI()
{
	if (ImGui::TreeNode(m_Name.c_str()))
	{
		ImGui::Begin(m_Name.c_str());

		ImGui::End();

		for (Actor* child : m_Childs)
		{
			child->DrawUI();
		}
		ImGui::TreePop();
	}
}
