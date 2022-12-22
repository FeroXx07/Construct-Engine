#ifndef _ComponentCollider_H
#define _ComponentCollider_H

#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

class btRigidBody;
class Module;
class GameObject;

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
	friend class ModuleSceneIntro;
public:
	ComponentCollider(btRigidBody* body);
	~ComponentCollider();

	void Update();
	void OnEditor();

	void Push(float x, float y, float z);
	void GetTransform(glm::mat4& mat) const;
	void SetTransform(glm::mat4& mat) const;
	void SetPos(float x, float y, float z);

	void SetTrigger(bool isTrigger);
	btRigidBody* GetBody();

	Shape m_Shape = Shape::CUBE;
	btRigidBody* m_Body = nullptr;
	std::list<Module*> m_Collision_listeners;
	bool m_Is_Trigger = false;
	bool m_Is_Static = true;

	glm::mat4x4 BtToGLM(btScalar* m, glm::mat4x4& dst)
	{
		// convert to (column major)
		dst[0][0] = m[0];
		dst[0][0] = m[0];
		dst[0][0] = m[0];
		dst[0][0] = m[0];

	}
};
#endif