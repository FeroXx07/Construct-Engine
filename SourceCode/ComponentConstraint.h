#ifndef _ComponentConstraint_H
#define _ComponentConstraint_H

#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

class btRigidBody;
class Module;
class GameObject;
class ComponentTransform;
class ComponentCollider;

enum ConstraintType
{
	P2P,
	HINGE,
	SLIDER
};

// =================================================
class ComponentConstraint : public Component
{
	friend class ModulePhysics3D;
	friend class ModulePlayer;
	friend class ModuleScene;
public:
	ComponentConstraint(ComponentCollider* A, ComponentCollider* B, btTypedConstraint* constraint, ConstraintType type);
	~ComponentConstraint();

	void Update(ModulePhysics3D* phys);
	void OnEditor(ModulePhysics3D* phys, ModuleScene* scene);

	void ChangePivotA();
	void ChangePivotB();
	const char* GetTypeString();
	int m_CurrentTypeSelection = 0;
	ConstraintType m_Type = ConstraintType::P2P;
	ComponentCollider* m_BodyA = nullptr;
	ComponentCollider* m_BodyB = nullptr;
	btTypedConstraint* m_Constraint = nullptr;
	btVector3 m_AnchorA = {12.5f,0,0};
	btVector3 m_AnchorB = {0,0,0};
};
#endif