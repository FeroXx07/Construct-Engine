#ifndef _PanelHierarchy_H
#define _PanelHierarchy_H
#include "Panel.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(ModuleScene* scene);
	~PanelHierarchy();

	void Start();
	void Update();

    void DrawGameObjNode(GameObject* node, ImGuiTreeNodeFlags flags);
    void ChangeSelectionNode(GameObject* newSelectedNode);
    void SetNullSelection();
    GameObject* GetSelectedNode();
    GameObject* m_SelectedNode = nullptr;
    GameObject* m_HoveredNode = nullptr;
	ModuleScene* m_Scene = nullptr;
	bool m_IsDraggingNode = false;
};


inline PanelHierarchy::PanelHierarchy(ModuleScene* scene) : Panel(true), m_Scene(scene)
{
}

inline PanelHierarchy::~PanelHierarchy()
{
	m_Scene = nullptr;
    m_SelectedNode = nullptr;
    m_HoveredNode = nullptr;
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
    node_flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    if (node->gui_IsSelected)
        node_flags |= ImGuiTreeNodeFlags_Selected;
    
    // Change state if clicked
    if (ImGui::TreeNodeEx(node->m_Name.c_str(), node_flags))
    {
        if (ImGui::IsItemHovered)
        {
            m_HoveredNode = node;
        }
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            ChangeSelectionNode(node);
        }

        if (ImGui::BeginDragDropSource())
        {
            string id = std::to_string(node->id);
            ImGui::SetDragDropPayload("hierarchy_node", id.c_str(), sizeof(id));
            ImGui::Text(node->m_Name.c_str());
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            //ImGuiDragDropFlags_SourceNoDisableHover
            const ImGuiPayload* data = ImGui::AcceptDragDropPayload("hierarchy_node");
            if (data != nullptr)
            {
                string idS = (const char*)data->Data;
                int idI = std::stoi(idS);
                GameObject* chl = this->m_Scene->root->FindById(idI);
                if (chl != nullptr)
                {
                    // Source
                    chl->SetParent(node);
                    node->SetChild(chl);
                    glm::mat4 inverse = glm::inverse(node->GetTransform()->GetWorld());
                    glm::mat4 mat = chl->GetTransform()->GetLocal();
                    inverse *= mat;
                    chl->GetTransform()->SetLocalMatrix(inverse);
                }
                //for (auto chl : this->m_Scene->root->m_Children)
                //{
                //    if (chl->id == idI)
                //    {
                //        // Source
                //        chl->SetParent(node);
                //        node->SetChild(chl);
                //        glm::mat4 inverse = glm::inverse(node->GetTransform()->GetWorld());
                //        glm::mat4 mat = chl->GetTransform()->GetLocal();
                //        inverse *= mat;
                //        chl->GetTransform()->SetLocalMatrix(inverse);
                //    }
                //}
            }
           
            ImGui::EndDragDropTarget();
        }

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
inline void PanelHierarchy::SetNullSelection()
{
    m_SelectedNode = nullptr;
}
inline GameObject* PanelHierarchy::GetSelectedNode()
{
    if (m_SelectedNode != nullptr)
        return m_SelectedNode;
    else
        return nullptr;
}
#endif