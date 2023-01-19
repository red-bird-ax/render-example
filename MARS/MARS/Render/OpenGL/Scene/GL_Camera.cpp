#include <Render/OpenGL/Scene/GL_Camera.h>

#include <glm/gtc/matrix_transform.hpp>

namespace OpenGL
{
	void Camera::BuildOrthographic(float aspectRatio, float near, float far)
	{
		m_Projection = glm::ortho(-1.f, 1.f, -aspectRatio, aspectRatio, near, far);
		UpdateViewMatrix();
	}

	void Camera::BuildPerspective(float fov, float aspectRatio, float near, float far)
	{
		m_Projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		m_View = m_Transform.GetModelMatrix();
	}
}