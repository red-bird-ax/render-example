#pragma once

#include <Scene/Scene.h>
#include <Math/Transform.h>

#include <Core.h>

enum class CameraType : uint8_t
{
	Perspective,
	Orthographic,
};

class Camera : public Actor
{
public:
	Camera(const std::string& name, const Transform& transform);
	virtual void DrawUI() override;
	virtual ActorType GetType() const override;

	void BuildPerspective(float fov, float aspectRatio, float near, float far);
	void BuildOrthographic(float aspectRatio, float near, float far);

	inline Matrix GetViewProjection() const { return m_Projection * m_View; }
	inline Matrix GetView()           const { return m_View; }

	void Rotate(const Rotator& rotation) { m_Transform.Rotation += rotation; UpdateViewMatrix(); }
	void Move(const Vector3D& offset) { m_Transform.Location += offset; UpdateViewMatrix(); }

	void SetLocation(const Vector3D& location) { m_Transform.Location = location; UpdateViewMatrix(); }
	void SetRotation(const Rotator& rotation) { m_Transform.Rotation = rotation; UpdateViewMatrix(); }

	inline void SetActive(bool active) { m_Active = active; }
	inline bool IsActive() const { return m_Active; }

private:
	void UpdateViewMatrix();

private:
	Matrix m_Projection;
	Matrix m_View;

	Transform m_Transform;
	bool m_Active = false;
};

