#include "ComponentCollider.h"
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
}

void ComponentCollider::Push(float x, float y, float z)
{
	if (m_Body != nullptr)
		m_Body->applyCentralImpulse(btVector3(x, y, z));
}

void ComponentCollider::GetTransform(glm::mat4& mat) const
{
	if (m_Body != nullptr)
		m_Body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(mat));
}

void ComponentCollider::SetTransform(glm::mat4& mat) const
{
	if (m_Body != nullptr)
	{
		btTransform t;
		t.setFromOpenGLMatrix(glm::value_ptr(mat));
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

btRigidBody* ComponentCollider::GetBody()
{
	return m_Body;
}
