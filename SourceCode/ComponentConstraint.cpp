#include "ComponentConstraint.h"
#include "GameObject.h"

ComponentConstraint::ComponentConstraint(ComponentCollider* A_, ComponentCollider* B_, btTypedConstraint* constraint_, ConstraintType type_) : Component(ComponentType::CONSTRAINT),
m_BodyA(A_), m_BodyB(B_), m_Constraint(constraint_), m_Type(type_)
{
}

ComponentConstraint::~ComponentConstraint()
{
	if (m_BodyA != nullptr)
		m_BodyA = nullptr;

	if (m_BodyB != nullptr)
		m_BodyB = nullptr;

	if (m_Constraint != nullptr)
		m_Constraint = nullptr;
}

void ComponentConstraint::Update(ModulePhysics3D* phys)
{
}

void ComponentConstraint::OnEditor(ModulePhysics3D* phys, ModuleScene* scene)
{
	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags lflag = ImGuiInputTextFlags_AllowTabInput;
	if (ImGui::TreeNodeEx("Constraint Information: ", treeFlags))
	{
		if (m_Type == ConstraintType::P2P)
		{
			if (ImGui::DragFloat3("Anchor point A", &m_AnchorA[0], 0.05f, 0.0f, 0.0f, "%.3f", lflag))
				ChangePivotA();
			ImGui::Separator();

			if (ImGui::DragFloat3("Anchor point B", &m_AnchorB[0], 0.05f, 0.0f, 0.0f, "%.3f", lflag))
				ChangePivotB();
			ImGui::Separator();
		}

		const char* items[] = { "P2P", "HINGE", "SLIDER" };
		struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
		ImGui::Combo("Constraint Type", &m_CurrentTypeSelection, &Funcs::ItemGetter, items, IM_ARRAYSIZE(items));
		if (m_CurrentTypeSelection != (int)m_Type)
		{
			m_Type = (ConstraintType)m_CurrentTypeSelection;
			// Call a function to change shape
			//phys->ChangeBodyShape(this, ConstraintType(m_CurrentTypeSelection));
		}
		ImGui::TreePop();
	}
}

void ComponentConstraint::ChangePivotA()
{
	if (m_Type == ConstraintType::P2P)
	{
		((btPoint2PointConstraint*)m_Constraint)->setPivotA(m_AnchorA);
	}
}

void ComponentConstraint::ChangePivotB()
{
	if (m_Type == ConstraintType::P2P)
	{
		((btPoint2PointConstraint*)m_Constraint)->setPivotA(m_AnchorB);
	}
}

const char* ComponentConstraint::GetTypeString()
{
	switch (m_Type)
	{
	case P2P:
		return "P2P";
		break;
	case HINGE:
		return "HINGE";
		break;
	case SLIDER:
		return "SLIDER";
		break;
	default:
		break;
	}
}
