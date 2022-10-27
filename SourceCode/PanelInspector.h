#ifndef _PanelInspector_H
#define _PanelInspector_H
#include "Panel.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"

class PanelInspector : Panel
{
public:
	PanelInspector();
	~PanelInspector();

	void Start();
	void Update();

	void SetNode(GameObject* node);
	GameObject* m_SelectedNode = nullptr;
};
inline PanelInspector::PanelInspector()
{

}

inline PanelInspector::~PanelInspector()
{
	m_SelectedNode = nullptr;
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