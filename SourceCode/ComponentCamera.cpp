#include "ComponentCamera.h"
#include "GameObject.h"
ComponentCamera::ComponentCamera(const char* name, glm::vec3 position) : Component(ComponentType::CAMERA), m_Name(name)
{
	camera = new Camera(position);
}

ComponentCamera::ComponentCamera(const char* name, Camera* camera_) : Component(ComponentType::CAMERA), m_Name(name), camera(camera_)
{
}

ComponentCamera::~ComponentCamera()
{
	if (camera != nullptr)
	{
		delete camera;
	}
	camera = nullptr;
}

void ComponentCamera::Update()
{
	if (this->m_hasGameObject)
	{
		camera->Position = this->m_GameObject->GetTransformConst()->GetTranslate();
	}
}

void ComponentCamera::OnEditor()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags iflags = ImGuiInputTextFlags_ReadOnly;
	if (ImGui::CollapsingHeader("Camera Information", flags))
	{
		ImGui::Text("Name: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", m_Name.c_str());
		ImGui::Separator();

		ImGui::Text("Zoom: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%f", camera->Zoom);
		ImGui::Separator();

		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%f %f %f", camera->Position.x, camera->Position.y, camera->Position.z);
		ImGui::Separator();
	}
}
