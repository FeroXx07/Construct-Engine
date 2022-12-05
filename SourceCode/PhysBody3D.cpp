#include "PhysBody3D.h"
//#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "GameObject.h"
// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : m_Body(body)
{}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	if (m_Body != nullptr)
	{
		delete m_Body;
		m_Body = nullptr;
	}
	
	ownerGameObject = nullptr;
}


// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	m_Body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(glm::mat4& mat) const
{
	if (m_Body != NULL)
	{
		m_Body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(mat))
			;
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(glm::mat4& mat) const
{
	if (m_Body != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(glm::value_ptr(mat));
		m_Body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = m_Body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	m_Body->setWorldTransform(t);
}

void PhysBody3D::SetGameObject(GameObject* go)
{
	ownerGameObject = go;
}

void PhysBody3D::SetAsSensor(bool is_sensor)
{
	if (this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if (is_sensor == true)
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}