#include <Scene/PointLight.h>

#include <External/imgui/imgui.h>

PointLight::PointLight(const std::string& name, const Transform& transform) : Actor{ name, transform }
{  }

void PointLight::Initailze(ID modelID, ID textureID)
{
    m_ModelID   = modelID;
    m_TextureID = textureID;
}

ActorType PointLight::GetType() const
{
    return ActorType::PointLight;
}

void PointLight::DrawUI()
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

void PointLight::Render()
{
	


































}
