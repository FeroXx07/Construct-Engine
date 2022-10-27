#include "ComponentMesh.h"
#include "Mesh.h"

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
		/*ImGui::InputFloat3("Position", &m_Translation[0], "%.3f", iflags);
		glm::vec3 euler;
		extractEulerAngleXYZ(m_TransMat, euler.x, euler.y, euler.z);
		euler = glm::degrees(euler);
		ImGui::InputFloat3("Rotation", &euler[0], "%.3f", iflags);
		ImGui::InputFloat3("Scaling", &m_Scaling[0], "%.3f", iflags);*/
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

//MeshSystem::MeshSystem()
//{
//}
//
//MeshSystem::~MeshSystem()
//{
//	m_ComponentsMeshList.clear();
//}
//
//void MeshSystem::AddComponentMeshToSystem(ComponentMesh* newC)
//{
//	bool alreadyExists = false;
//	for (auto c : m_ComponentsMeshList)
//		if (c == newC)
//			alreadyExists = true;
//
//	if (alreadyExists == false)
//	{
//		m_ComponentsMeshList.push_back(newC);
//	}
//}
//void MeshSystem::DrawAllMeshes(Shader& shader)
//{
//	//for (unsigned int i = 0; i < meshesList.size(); i++)
//	//	meshesList[i].Draw(shader);
//	for (auto m : m_ComponentsMeshList)
//		if (m->GetIsActive())
//			m->GetMesh()->Draw(shader);
//}

