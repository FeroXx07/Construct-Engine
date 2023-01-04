#ifndef _ComponentCollider_H
#define _ComponentCollider_H

#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "ComponentConstraint.h"

class btRigidBody;
class Module;
class GameObject;
class ComponentTransform;

enum Shape {
	SPHERE,
	CUBE,
	CYLINDER
};
// =================================================
class ComponentCollider : public Component
{
	friend class ModulePhysics3D;
	friend class ModulePlayer;
	friend class ModuleScene;
public:
	ComponentCollider(btRigidBody* body);
	~ComponentCollider();

	void Update(ModulePhysics3D* phys);
	void OnEditor(ModulePhysics3D* phys, ModuleScene* scene);

	void Push(float x, float y, float z);
	void GetTransform(glm::mat4& mat) const;
	void SetTransform(ComponentTransform* transform) const;
	void SetPos(float x, float y, float z);

	const char* GetShapeString();
	void SetTrigger(bool isTrigger);
	void SetStatic(bool isStatic);
	int CreateConstraint(ModulePhysics3D* phys, ComponentCollider* secondBody, ConstraintType type);
	btRigidBody* GetBody();

	glm::vec3 m_PositionOffset = glm::vec3(0.0f);
	glm::vec3 m_ScalingOffset = glm::vec3(1.0f);
	Shape m_Shape = Shape::CUBE;
	btRigidBody* m_Body = nullptr;
	std::list<Module*> m_Collision_listeners;
	bool m_Is_Trigger = false;
	bool m_Is_Static = false;
	float m_Mass = 1.0f;
	int m_CurrentShapeSelection = (int)m_Shape;
	GameObject* m_SelectionNode = nullptr;
};
#endif