#include "ComponentMesh.h"
#include "Mesh.h"
#include "GameObject.h"
ComponentMesh::ComponentMesh(Mesh* mesh) : Component(ComponentType::MESH), m_Mesh(nullptr)
{
	m_Mesh = mesh;
}

ComponentMesh::ComponentMesh() : Component(ComponentType::MESH), m_Mesh(nullptr)
{
}

ComponentMesh::~ComponentMesh()
{
	m_Mesh = nullptr;
}

void ComponentMesh::OnEditor()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags iflags = ImGuiInputTextFlags_ReadOnly;
	if (ImGui::CollapsingHeader("Mesh Information", flags))
	{
		ImGui::Checkbox("Render Normals", &m_DisplayNormals);
		ImGui::Text("Mesh name: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", m_Mesh->name.c_str());
		ImGui::Separator();

		ImGui::Text("Mesh vertices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", m_Mesh->vertices.size());
		ImGui::Separator();

		ImGui::Text("Mesh faces: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", m_Mesh->totalFaces);
		ImGui::Separator();
	}
}

void ComponentMesh::SetMesh(Mesh* mesh)
{
	m_Mesh = mesh;
}

Mesh* ComponentMesh::GetMesh()
{
	if (m_Mesh != nullptr)
		return m_Mesh;
	else
		return nullptr;
}


