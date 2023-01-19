#pragma once

#include <Render/OpenGL/Buffers/GL_UniformBuffer.h>
#include <Math/Transform.h>
#include <Math/Color.h>

#include <vector>
#include <string>

enum class ActorType : uint32_t
{
	Model,
	Camera,
	PointLight, DirectionalLight, Spotlight,
	Cube
};

class Scene;
class Model;
class Camera;
class PointLight;

class Actor
{
public:
	Actor(const std::string& name, const Transform& transform);

	inline void AddChild(Actor* actor) { m_Childs.push_back(actor); }
	inline std::vector<Actor*>& GetChilds() { return m_Childs; }

	inline Transform& GetTransform() { return m_Transform; }

	virtual void DrawUI();
	virtual ActorType GetType() const;

protected:
	Transform m_Transform;

	std::vector<Actor*> m_Childs;
	std::string m_Name;
};

class Scene
{
private:
	struct UIData
	{
		struct
		{
			bool Show = false;
			int32_t SelectedItemIdx = 0;

			struct {
				std::string Name;
				int32_t SelectdModelIdx = 0;
				Transform Transform;
				NormColor Diffuse;
				NormColor Ambient;
				NormColor Specular;
				float Shininess;
			} Model;

			struct {
				std::string Name;
				int32_t ProjectionIdx = 0;
				Transform Transform;
				float FOV;
				float Near;
				float Far;				
			} Camera;

			struct {
				std::string Name;
				float Intensity;
				Transform Transform;
				NormColor Diffuse;
				NormColor Ambient;
				NormColor Specular;
			} PointLight;

			struct {
				std::string Name;
				Transform Transform;
				NormColor Diffuse;
				NormColor Ambient;
				NormColor Specular;
			} DirLight;

			struct {
				std::string Name;
				Transform Transform;
				ID DiffuseID, SpecularID, EmissiveID;
				int32_t DiffuseIndex, SpecularIndex, EmissiveIndex;
			} Cube;

		} AddActorMenu;
	};

public:
	Scene(); // for test

	inline void AddChild(Actor* actor) { m_Root.push_back(actor); }

	void Initialize();
	void Render();
	
	inline const OpenGL::UniformBuffer& GetCameraUniform() const { return m_CameraUB; }
	inline const OpenGL::UniformBuffer& GetLightsUniform() const { return m_LightsUB; }
	inline const OpenGL::UniformBuffer& GetDirLightUniform() const { return m_DirectionalLightBuffer; }

	void DrawUI();
	void DeactivateCameras();

private:
	Camera* GetActiveCamera();
	void UpdateCamera(Camera* camera);

	std::vector<Model*>* GetActiveModels();

	std::vector<PointLight*>* GetActiveLights();
	void UpdateLights(std::vector<PointLight*>* lights);


private: // UI:
	void DrawAddActorMenu();
	void DrawActorTree();
	void DrawAddModel();
	void DrawAddCamera();
	void DrawAddCube();
	void DrawAddPointLight();
	void DrawAddDirectionalLight();

private:
	std::vector<Actor*> m_Root;
	
	OpenGL::UniformBuffer m_CameraUB;
	OpenGL::UniformBuffer m_LightsUB;
	OpenGL::UniformBuffer m_DirectionalLightBuffer;

	struct {
		Matrix ViewProjection;
		Matrix View;
		Vector3D CameraLocation;
	} m_CameraData;

	struct {
		NormColor Ambient;
		NormColor Diffuse;
		NormColor Specular;
		Vector3D  Location;
	} m_LightsData;

	struct {
		NormColor Ambient;
		NormColor Diffuse;
		NormColor Specular;
		Vector3D  Direction;
	} m_DirectionalLightPayload;

	UIData m_UIData;

	//std::vector<Model*> m_ModelsToDraw;
};

void UpdateTransform(Transform& transform);
void UpdateColor(NormColor& color, const std::string& title);
void UpdateString(std::string& string, const std::string& title);

/*
	Ambient: color, intensity (color * intensity)
	Point: color, intinsity, location

*/