#ifndef _PanelHierarchy_H
#define _PanelHierarchy_H
#include "Panel.h"
#include "ModuleScene.h"

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(ModuleScene* scene);
	~PanelHierarchy();

	void Start();
	void Update();

    void DrawGameObjNode(GameObject* node, ImGuiTreeNodeFlags flags);
    void ChangeSelectionNode(GameObject* newSelectedNode);
    GameObject* GetSelectedNode();
    GameObject* m_SelectedNode = nullptr;
	ModuleScene* m_Scene = nullptr;
	bool test_drag_and_drop = true;
};


inline PanelHierarchy::PanelHierarchy(ModuleScene* scene) : m_Scene(scene)
{
}

inline PanelHierarchy::~PanelHierarchy()
{
	m_Scene = nullptr;
    m_SelectedNode = nullptr;
}

inline void PanelHierarchy::Start()
{
}

inline void PanelHierarchy::Update()
{
	ImGui::Begin("GameObjects Hierarchy");
	ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
    if (m_Scene != nullptr && m_Scene->root != nullptr)
    {
        GameObject* root = m_Scene->root;
        DrawGameObjNode(root, base_flags);
    }
	ImGui::End();
}

inline void PanelHierarchy::DrawGameObjNode(GameObject* node, ImGuiTreeNodeFlags flags)
{
    // Flags for selected nodes
    ImGuiTreeNodeFlags node_flags = flags;
    if (node->gui_IsSelected)
        node_flags |= ImGuiTreeNodeFlags_Selected;

    // Change state if clicked
    // bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
    bool node_open = ImGui::TreeNodeEx(node->m_Name.c_str(), node_flags);

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        LOG("Node: %s is selected", node->m_Name.c_str());
        ChangeSelectionNode(node);
    }
        

    if (test_drag_and_drop && ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
        ImGui::Text("This is a drag and drop source");
        ImGui::EndDragDropSource();
    }
    if (node_open)
    {
        for (auto children : node->m_Children)
        {
            DrawGameObjNode(children, flags);
        }
        ImGui::TreePop();
    }
}
inline void PanelHierarchy::ChangeSelectionNode(GameObject* newSelectedNode)
{
    if (m_SelectedNode != nullptr)
        m_SelectedNode->gui_IsSelected = false;

    if (newSelectedNode != nullptr)
        newSelectedNode->gui_IsSelected = true;

    m_SelectedNode = newSelectedNode;
}
inline GameObject* PanelHierarchy::GetSelectedNode()
{
    if (m_SelectedNode != nullptr)
        return m_SelectedNode;
    else
        return nullptr;
}
#endif