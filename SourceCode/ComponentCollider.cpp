#include "ComponentCollider.h"
#include "ComponentTransform.h"
#include "ComponentConstraint.h"
#include "GameObject.h"
#include "ModulePhysics3D.h"
#include "ModuleScene.h"
	
ComponentCollider::ComponentCollider(btRigidBody* body) : Component(ComponentType::COLLIDER), m_Body(body), m_SelectionNode(nullptr)
{
}

ComponentCollider::~ComponentCollider()
{
	m_SelectionNode = nullptr;
	if (m_Body != nullptr)
	{
		m_Body = nullptr;
	}
}

void ComponentCollider::Update(ModulePhysics3D* phys)
{
	if (m_hasGameObject)
	{
		glm::mat4 mat;
		GetTransform(mat);
		m_GameObject->GetTransform()->SetLocalMatrix(mat);

		m_Body->getCollisionShape()->setLocalScaling((btVector3(m_ScalingOffset.x, m_ScalingOffset.y, m_ScalingOffset.z)));
		phys->world->updateSingleAabb(m_Body);
	}
}

void ComponentCollider::OnEditor(ModulePhysics3D* phys, ModuleScene* scene)
{
	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags lflag = ImGuiInputTextFlags_AllowTabInput;
	ImGuiInputTextFlags readOnlyFlag = ImGuiInputTextFlags_ReadOnly;
	if (ImGui::TreeNodeEx("Collider Information: ", treeFlags))
	{
		ImGui::DragFloat3("Collider Offset", &m_PositionOffset[0], 0.05f, 0.0f, 0.0f, "%.3f", lflag);
		ImGui::Separator();

		ImGui::DragFloat3("Scalling Offset", &m_ScalingOffset[0], 0.05f, 0.0f, 0.0f, "%.3f", lflag);
		ImGui::Separator();

		if (ImGui::DragFloat("Mass", &m_Mass, 0.05f, 0.0f, 0.0f, "%.3f", lflag))
		{
			m_Body->setMassProps(m_Mass, { 0,0,0 });
		}
		ImGui::Separator();

		bool isStaticLocal = m_Is_Static;
		ImGui::Checkbox("Is Static", &isStaticLocal);
		if (isStaticLocal != m_Is_Static)
			SetStatic(isStaticLocal);
		ImGui::Separator();

		bool isTriggerLocal = m_Is_Trigger;
		ImGui::Checkbox("Is Trigger", &isTriggerLocal);
		if (isTriggerLocal != m_Is_Trigger)
			SetTrigger(isTriggerLocal);
		ImGui::Separator();

		const char* items[] = { "SPEHRE", "CUBE", "CYLINDER"};
		struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
		ImGui::Combo("Collider Shape", &m_CurrentShapeSelection, &Funcs::ItemGetter, items, IM_ARRAYSIZE(items));
		if (m_CurrentShapeSelection != (int)m_Shape)
		{
			// Call a function to change shape
			phys->ChangeBodyShape(this, Shape(m_CurrentShapeSelection));
		}

		static int e = 0;
		ImGui::RadioButton("Point to Point", &e, 0); ImGui::SameLine();
		ImGui::RadioButton("Hinge", &e, 1); ImGui::SameLine();
		ImGui::RadioButton("Slider", &e, 2);

		string name = "Drag and drop a GameObject";
		if (m_SelectionNode != nullptr)
			name = m_SelectionNode->m_Name;

		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen;
		node_flags |= ImGuiTreeNodeFlags_Bullet;
		node_flags |= ImGuiTreeNodeFlags_Selected;
		// Change state if clicked
		if (ImGui::TreeNodeEx(name.c_str(), node_flags))
		{
			if (ImGui::BeginDragDropTarget())
			{
				//ImGuiDragDropFlags_SourceNoDisableHover
				const ImGuiPayload* data = ImGui::AcceptDragDropPayload("hierarchy_node");
				if (data != nullptr)
				{
					string idS = (const char*)data->Data;
					int idI = std::stoi(idS);
					m_SelectionNode = scene->root->FindById(idI);
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::TreePop();
		}

		// Flags for selected nodes
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
		if (ImGui::Button("Create Constraint Component"))
		{
			if (m_SelectionNode != nullptr && m_GameObject->m_HasComponentConstraint == false)
			{
				if (m_SelectionNode->m_HasComponentCollider)
					CreateConstraint(phys, m_SelectionNode->GetCollider(), (ConstraintType)e);
			}
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();

		ImGui::TreePop();
	}
	
}

void ComponentCollider::Push(float x, float y, float z)
{
	if (m_Body != nullptr)
		m_Body->applyCentralImpulse(btVector3(x, y, z));
}

void ComponentCollider::GetTransform(glm::mat4& mat) const
{
	// Physics to Engine
	// Set the position offset
	if (m_Body != nullptr)
	{
		btTransform offsetedTransform(m_Body->getWorldTransform());

		ComponentTransform* t = m_GameObject->GetTransformConst();

		// Get the transform fromn physyics
		btVector3 posBullet = offsetedTransform.getOrigin();
		btQuaternion rotBullet = offsetedTransform.getRotation();

		glm::quat rotEngine = glm::quat(rotBullet.w(), rotBullet.x(), rotBullet.y(), rotBullet.z());
		glm::vec3 finalOffset = rotEngine * m_PositionOffset;
		glm::vec3 posEngine = glm::vec3(posBullet.x() - finalOffset.x, posBullet.y() - finalOffset.y, posBullet.z() - finalOffset.z);

		// Remove the offset because offset is internal only(collider wise)
		offsetedTransform.setOrigin(btVector3(posEngine.x, posEngine.y, posEngine.z));
		offsetedTransform.setRotation(rotBullet);
		offsetedTransform.getOpenGLMatrix(glm::value_ptr(mat));
	}
}

void ComponentCollider::SetTransform(ComponentTransform* transform) const
{
	// Engine to Physics
	// Set the position offset
	if (m_Body != nullptr)
	{
		// Get the position from the engine
		glm::vec3 posEngine = transform->GetTranslate();;
		glm::quat rotEngine = transform->GetRotationQuat();

		// Get the offset
		glm::vec3 finalOffset = rotEngine * m_PositionOffset;
		glm::vec3 finalPosBullet = posEngine + finalOffset;

		// Apply the offset because offset is internal only(collider wise)
		btTransform t;
		t.setFromOpenGLMatrix(glm::value_ptr(transform->GetWorld()));
		t.setOrigin(btVector3(finalPosBullet.x, finalPosBullet.y, finalPosBullet.z));
		t.setRotation(btQuaternion(rotEngine.x, rotEngine.y, rotEngine.z, rotEngine.w));

		m_Body->setWorldTransform(t);
	}
}

void ComponentCollider::SetPos(float x, float y, float z)
{
	if (m_Body != nullptr)
	{
		btTransform t = m_Body->getWorldTransform();
		t.setOrigin(btVector3(x, y, z));
		m_Body->setWorldTransform(t);
	}
}

const char* ComponentCollider::GetShapeString()
{
	switch (m_Shape)
	{
	case SPHERE:
		return "SPHERE";
		break;
	case CUBE:
		return "CUBE";
		break;
	case CYLINDER:
		return "CYLINDER";
		break;
	default:
		return "NONE";
		break;
	}
}

void ComponentCollider::SetTrigger(bool isTrigger)
{
	if (m_Is_Trigger != isTrigger)
	{
		m_Is_Trigger = isTrigger;
		if (m_Is_Trigger == true)
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

void ComponentCollider::SetStatic(bool isStatic)
{
	if (m_Is_Static != isStatic)
	{
		m_Is_Static = isStatic;
		if (m_Is_Static == true)
		{
			this->m_Body->setMassProps(0.0f, { 0,0,0 });
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		}
		else
		{
			this->m_Body->setMassProps(1.0f, { 0,0,0 });
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
		}
	}
}

int ComponentCollider::CreateConstraint(ModulePhysics3D* phys, ComponentCollider* secondBody, ConstraintType type)
{	
	ComponentConstraint* constraint = nullptr;
	switch (type)
	{
	case P2P:
	{
		constraint = new ComponentConstraint(this, secondBody, phys->AddConstraintP2P(*this, *secondBody, { 12.5f,0,0 }, { 0,0,0 }), ConstraintType::P2P);
		break;
	}
	case HINGE:
	{
		constraint = new ComponentConstraint(this, secondBody, phys->AddConstraintHinge(*this, *secondBody, { 12.5f,0,0 }, { 0,0,0 }, { 1,0,0 }, { 1,0,0 }), ConstraintType::HINGE);
		break;
	}
	case SLIDER:
	{
		constraint = new ComponentConstraint(this, secondBody, phys->AddConstraintSlider(*this, *secondBody), ConstraintType::SLIDER);
		break;
	}
	default:
	{
		constraint = new ComponentConstraint(this, secondBody, phys->AddConstraintP2P(*this, *secondBody, { 0,0,0 }, { 0,0,0 }), ConstraintType::P2P);
		break;
	}
	}
	m_GameObject->AssignComponent(constraint);
	secondBody->m_GameObject->AssignComponent(constraint);
	m_Body->updateInertiaTensor();
	secondBody->m_Body->updateInertiaTensor();
	phys->world->updateSingleAabb(m_Body);
	phys->world->updateSingleAabb(secondBody->m_Body);
	phys->ForceUpdate();
	return 1;
}

btRigidBody* ComponentCollider::GetBody()
{
	return m_Body;
}
