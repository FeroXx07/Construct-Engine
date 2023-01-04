#pragma once
#include "Module.h"
#include "Globals.h"
#include <list>
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "ComponentCollider.h"
#include "ComponentConstraint.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
struct PhysVehicle3D;
struct VehicleInfo;

class ModulePhysics3D : public Module
{
	friend class ComponentConstraint;
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	void ForceUpdate();
	bool CleanUp();

	ComponentCollider* AddBodySphere(glm::mat4x4 transform, float radius = 1.0f, float mass = 1.0f, bool isStatic = false);
	ComponentCollider* AddBodyCube(const math::AABB& box, glm::mat4x4 transform, float mass = 1.0f, bool isStatic = false);
	ComponentCollider* AddBodyCube(const glm::vec3& halfExtents, glm::mat4x4 transform, float mass = 1.0f, bool isStatic = false);
	ComponentCollider* AddBodyCylinder(glm::mat4x4 transform, float height, float radius, float mass = 1.0f, bool isStatic = false);
	void ChangeBodyShape(ComponentCollider* body, Shape newShape);
	void DeleteShape(ComponentCollider* body);
	void ChangeConstraint(ComponentConstraint* constraint, ConstraintType newType);
	/*PhysVehicle3D* AddVehicle(const VehicleInfo& info);*/

	ComponentCollider* ground;
	btTypedConstraint* AddConstraintP2P(ComponentCollider& bodyA, ComponentCollider& bodyB, const vec3& anchorA, const vec3& anchorB);
	btTypedConstraint* AddConstraintHinge(ComponentCollider& bodyA, ComponentCollider& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, bool disable_collision = false);
	btTypedConstraint* AddConstraintSlider(ComponentCollider& bodyA, ComponentCollider& bodyB);
	btTypedConstraint* AddConstraintSixDof(btRigidBody& bodyA, btRigidBody& bodyB, const btTransform& transform1, const btTransform& transform2);
	btDiscreteDynamicsWorld* world;
	float delta = 1.0f / 60.0f;
private:

	bool debug = true;

	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	
	btDefaultVehicleRaycaster*			vehicle_raycaster;
	DebugDrawer*						debug_draw;

	std::list<btCollisionShape*> shapes;
	std::list<ComponentCollider*> bodies;
	std::list<btDefaultMotionState*> motions;
	std::list<btTypedConstraint*> constraints;
	std::list<PhysVehicle3D*> vehicles;
};


class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	P_Line line;
	Primitive point;
};