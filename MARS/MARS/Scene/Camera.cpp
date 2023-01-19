#include <Scene/Camera.h>

#include <External/imgui/imgui.h>

#include <Global.h>

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const std::string& name, const Transform& transform) : Actor{ name, transform }
{
	m_Projection = Matrix{ 1 };
	m_View = Matrix{ 1 };

	AppScene->DeactivateCameras();
	m_Active = true;
}

ActorType Camera::GetType() const { return ActorType::Camera; }

void Camera::BuildPerspective(float fov, float aspectRatio, float near, float far)
{
	m_Projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	UpdateViewMatrix();
}

void Camera::BuildOrthographic(float aspectRatio, float near, float far)
{
	m_Projection = glm::ortho(-1.f, 1.f, -aspectRatio, aspectRatio, near, far);
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	m_View = m_Transform.GetModelMatrix();
}

void Camera::DrawUI()
{
	if (ImGui::TreeNode(m_Name.c_str()))
	{
		ImGui::Begin(m_Name.c_str());

		UpdateTransform(m_Transform);
		UpdateViewMatrix();

		if (!m_Active)
		{
			if (ImGui::Button("Activate"))
			{
				AppScene->DeactivateCameras();
				m_Active = true;
			}
		}
		else
		{
			ImGui::Text("Camera is active");
		}	

		ImGui::End();

		for (Actor* child : m_Childs)
		{
			child->DrawUI();
		}
		ImGui::TreePop();
	}
}