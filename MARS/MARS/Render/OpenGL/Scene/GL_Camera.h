#pragma once

#include <Math/Transform.h>

#include <cstdint>

namespace OpenGL
{
	class Camera
	{
	public:
		void BuildOrthographic(float aspectRatio, float near, float far);
		void BuildPerspective(float fov, float aspectRatio, float near, float far);

		inline Matrix GetViewProjection() const { return m_Projection * m_View; }
		inline Matrix GetView()           const { return m_View; }

		void Rotate(const Rotator& rotation) { m_Transform.Rotation += rotation; UpdateViewMatrix(); }
		void Move(const Vector3D& offset) { m_Transform.Location += offset; UpdateViewMatrix(); }

		void SetLocation(const Vector3D& location) { m_Transform.Location = location; UpdateViewMatrix(); }
		void SetRotation(const Rotator& rotation) { m_Transform.Rotation = rotation; UpdateViewMatrix(); }

	private:
		void UpdateViewMatrix();

	private:
		Matrix m_Projection;
		Matrix m_View;

		Transform m_Transform;
	};
}