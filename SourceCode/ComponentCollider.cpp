#include "ComponentCollider.h"
#include "ComponentTransform.h"
#include "GameObject.h"

ComponentCollider::ComponentCollider(btRigidBody* body) : Component(ComponentType::COLLIDER), m_Body(body)
{
}

ComponentCollider::~ComponentCollider()
{
	if (m_Body != nullptr)
	{
		delete m_Body;
		m_Body = nullptr;
	}
}

void ComponentCollider::Update()
{
	if (m_hasGameObject)
	{
		glm::mat4 mat;
		GetTransform(mat);
		m_GameObject->GetTransform()->SetLocalMatrix(mat);
	}
}

void ComponentCollider::OnEditor()
{
	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags lflag = ImGuiInputTextFlags_AllowTabInput;
	ImGuiInputTextFlags readOnlyFlag = ImGuiInputTextFlags_ReadOnly;
	if (ImGui::TreeNodeEx("Collider Information: ", treeFlags))
	{
		ImGui::DragFloat3("Collider Offset", &positionOffset[0], 0.05f, 0.0f, 0.0f, "%.3f", lflag);
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
		glm::vec3 finalOffset = rotEngine * positionOffset;
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
		glm::vec3 finalOffset = rotEngine * positionOffset;
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

btRigidBody* ComponentCollider::GetBody()
{
	return m_Body;
}
