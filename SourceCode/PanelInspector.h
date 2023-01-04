#ifndef _PanelInspector_H
#define _PanelInspector_H
#include "Panel.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "ModulePhysics3D.h"
#include "ModuleScene.h"
class PanelInspector : Panel
{
public:
	PanelInspector(ModulePhysics3D* phys, ModuleScene* scene) : Panel(true), isActive(true), m_Phys(phys), m_Scene(scene) {};
	~PanelInspector();

	void Start();
	void Update();

	void SetNode(GameObject* node);
	GameObject* m_SelectedNode = nullptr;
	ModulePhysics3D* m_Phys = nullptr;
	ModuleScene* m_Scene = nullptr;
	bool isActive = false;
};

inline PanelInspector::~PanelInspector()
{
	m_SelectedNode = nullptr;
	m_Phys = nullptr;
	m_Scene = nullptr;
}

inline void PanelInspector::Start()
{
}

inline void PanelInspector::Update()
{
	ImGui::Begin("Properties Inspector");
	ImGui::Text("Options");
	ImGui::Separator();
	if (m_SelectedNode != nullptr)
	{
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
		ImGui::InputText("Object Name", &m_SelectedNode->m_Name, flags);

		if (m_SelectedNode->m_HasComponentTransform)
			m_SelectedNode->GetTransform()->OnEditor();

		if (m_SelectedNode->m_HasComponentMesh)
			m_SelectedNode->GetMesh()->OnEditor();
	
		if (m_SelectedNode->m_HasComponentMaterial)
			m_SelectedNode->GetMaterial()->OnEditor();

		if (m_SelectedNode->m_HasComponentCamera)
			m_SelectedNode->GetCamera()->OnEditor();

		if (m_SelectedNode->m_HasComponentCollider)
			m_SelectedNode->GetCollider()->OnEditor(m_Phys, m_Scene);

		if (m_SelectedNode->m_HasComponentConstraint)
			m_SelectedNode->GetConstraint()->OnEditor(m_Phys, m_Scene);
	}
	else
	{
		ImGui::TextWrapped("Please select any node!");
	}
	
	ImGui::End();
}

inline void PanelInspector::SetNode(GameObject* node)
{
	if (node != nullptr)
		m_SelectedNode = node;
}

#endif