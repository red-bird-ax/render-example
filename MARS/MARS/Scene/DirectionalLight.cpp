#include <Scene/DirectionalLight.h>

#include <External/imgui/imgui.h>

DirectionalLight::DirectionalLight(const std::string& name, const Transform& transform) : Actor{ name, transform } { }

ActorType DirectionalLight::GetType() const { return ActorType::DirectionalLight; }

Vector3D DirectionalLight::GetDirection() const
{
	const Matrix inverted = glm::inverse(m_Transform.GetModelMatrix());
	const glm::vec3 forward = glm::normalize(glm::vec3(inverted[2]));
	return Vector3D{ forward.x, forward.y, forward.z };
}

void DirectionalLight::DrawUI()
{
	if (ImGui::TreeNode(m_Name.c_str()))
	{
		ImGui::Begin(m_Name.c_str());

		UpdateTransform(m_Transform);
		UpdateString(m_Name, "Name");

		ImGui::Separator();
		if (ImGui::TreeNode("Parameters"))
		{
			UpdateColor(m_Properties.Diffuse, "Diffuse Color");
			UpdateColor(m_Properties.Ambient, "Ambient Color");
			UpdateColor(m_Properties.Specular, "Specular Color");
			ImGui::TreePop();
		}

		ImGui::End();

		for (Actor* child : m_Childs)
		{
			child->DrawUI();
		}
		ImGui::TreePop();
	}
}