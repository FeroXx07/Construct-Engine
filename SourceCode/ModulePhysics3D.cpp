#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "Primitive.h"
#include <algorithm>

#ifdef _DEBUG
	#pragma comment (lib, "SourceCode/Bullet/lib/BulletCollision_vs2010_x64_debug.lib")
	#pragma comment (lib, "SourceCode/Bullet/lib/BulletDynamics_vs2010_x64_debug.lib")
	#pragma comment (lib, "SourceCode/Bullet/lib/LinearMath_vs2010_x64_debug.lib")
#else
	#pragma comment (lib, "SourceCode/Bullet/lib/BulletCollision_vs2010_x64_release.lib")
	#pragma comment (lib, "SourceCode/Bullet/lib/BulletDynamics_vs2010_x64_release.lib")
	#pragma comment (lib, "SourceCode/Bullet/lib/LinearMath_vs2010_x64_release.lib")
#endif
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;

	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	debug_draw->setDebugMode(debug_draw->DBG_MAX_DEBUG_DRAW_MODE);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY*2);
	//vehicle_raycaster = new btDefaultVehicleRaycaster(world);
	// Big plane as ground
	{
		btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		ground = new ComponentCollider(body);
		body->setUserPointer(ground);
		world->addRigidBody(body);

	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	if (App->scene->editorState == StateEditor::ON_PLAYING)
	{
		world->stepSimulation(delta, 15);

		int numManifolds = world->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
			btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

			int numContacts = contactManifold->getNumContacts();
			if (numContacts > 0)
			{
				ComponentCollider* pbodyA = (ComponentCollider*)obA->getUserPointer();
				ComponentCollider* pbodyB = (ComponentCollider*)obB->getUserPointer();

				if (pbodyA && pbodyB)
				{
					//std::list<Module*>* item = pbodyA->collision_listeners.getFirst();
					for (std::list<Module*>::iterator item = pbodyA->m_Collision_listeners.begin(); item != pbodyA->m_Collision_listeners.end(); item++)
					{
						(*item)->OnCollision(pbodyA, pbodyB);
					}

					for (std::list<Module*>::iterator item2 = pbodyB->m_Collision_listeners.begin(); item2 != pbodyB->m_Collision_listeners.end(); item2++)
					{
						(*item2)->OnCollision(pbodyB, pbodyA);
					}
					//while(*item && *item != nullptr)
					//{
					//	(*item)->OnCollision(pbodyA, pbodyB);
					//	//item->data->OnCollision(pbodyA, pbodyB);
					//	item++;
					//}

					//item = pbodyB->collision_listeners.begin();
					//while(*item && *item != nullptr)
					//{
					//	(*item)->OnCollision(pbodyB, pbodyA);
					//	//item->data->OnCollision(pbodyA, pbodyB);
					//	item++;
					//}
				}
			}
		}

	}

	
	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		for (std::list<ComponentCollider*>::iterator item = bodies.begin(); item != bodies.end(); item++)
		{
			(*item)->Push(0.0f,200.0f,0.0f);
		}
	}
	for (std::list<ComponentCollider*>::iterator item = bodies.begin(); item != bodies.end(); item++)
	{
		(*item)->Update(this);
		glm::mat4x4 m;
		(*item)->GetTransform(m);
		string n = (*item)->m_GameObject->m_Name;
		//std::cout << n << "  " << glm::to_string(m) << std::endl << std::endl;
	}

	if(debug == true)
	{
		//world->debugDrawWorld();

		// Render vehicles
		//std::list<PhysVehicle3D*>::iterator item = vehicles.begin();
		////p2List_item<PhysVehicle3D*>* item = vehicles.getFirst();
		//while(*item && *item != nullptr)
		//{
		//	/*(*item)->Render();*/
		//	/**item->Render();
		//	++i;*/
		//}

		/*if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			Sphere s(1);
			s.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			float force = 30.0f;
			AddBody(s)->Push(-(App->camera->Z.x * force), -(App->camera->Z.y * force), -(App->camera->Z.z * force));
		}*/
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModulePhysics3D::ForceUpdate()
{
	world->stepSimulation(1.0f / 120.0f, 15);
	for (std::list<ComponentCollider*>::iterator item = bodies.begin(); item != bodies.end(); item++)
	{
		(*item)->Update(this);
		glm::mat4x4 m;
		(*item)->GetTransform(m);
		string n = (*item)->m_GameObject->m_Name;
		//std::cout << n << "  " << glm::to_string(m) << std::endl << std::endl;
	}
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}
	
	for(std::list<btTypedConstraint*>::iterator item = constraints.begin(); item != constraints.end(); item++)
	{
		ComponentCollider* collA = (ComponentCollider*)(*item)->getRigidBodyA().getUserPointer();
		ComponentCollider* collB = (ComponentCollider*)(*item)->getRigidBodyB().getUserPointer();
		ComponentConstraint* constr = collA->m_GameObject->GetConstraint();
		collA->m_GameObject->DeAssignComponent(ComponentType::CONSTRAINT);
		collB->m_GameObject->DeAssignComponent(ComponentType::CONSTRAINT);
		delete constr;
		world->removeConstraint((*item));
		delete *item;
		*item = nullptr;
	}
	
	constraints.clear();

	/*for(std::list<btDefaultMotionState*>::iterator item = motions.begin(); item != motions.end(); item++)
	{
		delete *item;
		*item = nullptr;
	}

	motions.clear();*/

	/*for (std::list<btCollisionShape*>::iterator item = shapes.begin(); item != shapes.end(); item++)
	{
		delete* item;
		*item = nullptr;
	}

	shapes.clear();*/

	for (std::list<ComponentCollider*>::iterator item = bodies.begin(); item != bodies.end(); item++)
	{
		//(*item)->m_GameObject->DeAssignComponent(ComponentType::COLLIDER);
		DeleteShape(*item);
		delete* item;
		*item = nullptr;
	}

	bodies.clear();

	//for (std::list<PhysVehicle3D*>::iterator item = vehicles.begin(); item != vehicles.end(); ++item)
	//{
	//	delete* item;
	//	*item = nullptr;
	//}

	//vehicles.clear();

	//delete vehicle_raycaster;
	delete world;

	return true;
}

// ---------------------------------------------------------
ComponentCollider* ModulePhysics3D::AddBodySphere(glm::mat4x4 transform, float radius, float mass, bool isStatic)
{
	btCollisionShape* colShape = new btSphereShape(radius);
	shapes.push_back(colShape);
	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(glm::value_ptr(transform));

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	ComponentCollider* pbody = new ComponentCollider(body);

	pbody->m_Shape = Shape::SPHERE;
	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);
	pbody->m_Body = body;
	//sphere.body = pbody;
	//pbody->parentPrimitive = (Primitive*)&sphere;
	return pbody;
}


// ---------------------------------------------------------
ComponentCollider* ModulePhysics3D::AddBodyCube(const math::AABB& box, glm::mat4x4 transform, float mass, bool isStatic)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::vec3 Scaling;
	glm::quat Rotation;
	glm::vec3 Translation;
	glm::decompose(transform, Scaling, Rotation, Translation, skew, perspective);
	
	// Get the size of the box
	math::float3 halfExtents = box.HalfSize();
	btCollisionShape* colShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
	
	shapes.push_back(colShape);
	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(glm::value_ptr(transform));

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	
	btRigidBody* body = new btRigidBody(rbInfo);
	ComponentCollider* pbody = new ComponentCollider(body);
	pbody->m_PositionOffset.z = halfExtents.z;

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);
	pbody->m_Body = body;
	pbody->m_Shape = Shape::CUBE;
	/*cube.body = pbody;*/
	/*pbody->parentPrimitive = (Primitive*)&cube;*/
	return pbody;
}

ComponentCollider* ModulePhysics3D::AddBodyCube(const glm::vec3& halfExtents, glm::mat4x4 transform, float mass, bool isStatic)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::vec3 Scaling;
	glm::quat Rotation;
	glm::vec3 Translation;
	glm::decompose(transform, Scaling, Rotation, Translation, skew, perspective);
	btCollisionShape* colShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(glm::value_ptr(transform));

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	ComponentCollider* pbody = new ComponentCollider(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);
	pbody->m_Body = body;
	pbody->m_Shape = Shape::CUBE;
	return pbody;
}

// ---------------------------------------------------------
ComponentCollider* ModulePhysics3D::AddBodyCylinder(glm::mat4x4 transform, float height, float radius, float mass, bool isStatic)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(height*0.5f, radius, 0.0f));
	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(glm::value_ptr(transform));

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	ComponentCollider* pbody = new ComponentCollider(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);
	pbody->m_Body = body;
	pbody->m_Shape = Shape::CYLINDER;
	/*cylinder.body = pbody;
	pbody->parentPrimitive = (Primitive*)&cylinder;*/
	return pbody;
}

void ModulePhysics3D::ChangeBodyShape(ComponentCollider* body, Shape newShape)
{
	// Remove the body from the world
	world->removeRigidBody(body->m_Body);
	// Delete the old shape
	shapes.remove(body->m_Body->getCollisionShape());
	delete body->m_Body->getCollisionShape();
	btCollisionShape* colShape = nullptr;
	// Create new Shape
	switch (newShape)
	{
	case SPHERE:
	{
		colShape = new btSphereShape(body->m_GameObject->m_Aabb.MaximalContainedSphere().r);
		body->m_Shape = Shape::SPHERE;
		break;
	}
	case CUBE:
	{
		// Get the size of the box
		math::float3 halfExtents = body->m_GameObject->m_Aabb.HalfSize();
		colShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
		body->m_Shape = Shape::CUBE;
		break;
	}
	case CYLINDER:
	{
		colShape = new btCylinderShapeX(btVector3(body->m_GameObject->m_Aabb.maxPoint.y - body->m_GameObject->m_Aabb.minPoint.y * 0.5f, 1.0f, 0.0f));
		body->m_Shape = Shape::CYLINDER;
		break;
	}
	default:
		break;
	}
	shapes.push_back(colShape);
	body->m_Body->setCollisionShape(colShape);
	colShape->calculateLocalInertia(1.0f, btVector3(0, 0, 0));
	body->m_Body->setMassProps(1.0f, btVector3(0, 0, 0));
	body->m_Body->updateInertiaTensor();
	world->addRigidBody(body->m_Body);
}

void ModulePhysics3D::DeleteShape(ComponentCollider* body)
{
	int constrainNums = body->m_Body->getNumConstraintRefs();
	if (constrainNums > 0)
	{
		for (int i = 0; i < constrainNums; i++)
		{
			constraints.remove(body->m_Body->getConstraintRef(i));
			world->removeConstraint(body->m_Body->getConstraintRef(i));
			delete body->m_Body->getConstraintRef(i);
		}
	}

	motions.remove((btDefaultMotionState*)body->m_Body->getMotionState());
	delete body->m_Body->getMotionState();

	shapes.remove(body->m_Body->getCollisionShape());
	delete body->m_Body->getCollisionShape();

	world->removeRigidBody(body->m_Body);
	delete body->m_Body;

	body->RemoveComponentFromGameObject();
}

// ---------------------------------------------------------
//PhysVehicle3D* ModulePhysics3D::AddVehicle(const VehicleInfo& info)
//{
//	btCompoundShape* comShape = new btCompoundShape();
//	shapes.push_back(comShape);
//
//	btCollisionShape* colShape = new btBoxShape(btVector3(info.chassis_size.x*0.5f, info.chassis_size.y*0.5f, info.chassis_size.z*0.5f));
//	shapes.push_back(colShape);
//
//	btTransform trans;
//	trans.setIdentity();
//	trans.setOrigin(btVector3(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z));
//
//	comShape->addChildShape(trans, colShape);
//
//	btTransform startTransform;
//	startTransform.setIdentity();
//
//	btVector3 localInertia(0, 0, 0);
//	comShape->calculateLocalInertia(info.mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(info.mass, myMotionState, comShape, localInertia);
//
//	btRigidBody* body = new btRigidBody(rbInfo);
//	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
//	body->setActivationState(DISABLE_DEACTIVATION);
//
//	world->addRigidBody(body);
//
//	btRaycastVehicle::btVehicleTuning tuning;
//	tuning.m_frictionSlip = info.frictionSlip;
//	tuning.m_maxSuspensionForce = info.maxSuspensionForce;
//	tuning.m_maxSuspensionTravelCm = info.maxSuspensionTravelCm;
//	tuning.m_suspensionCompression = info.suspensionCompression;
//	tuning.m_suspensionDamping = info.suspensionDamping;
//	tuning.m_suspensionStiffness = info.suspensionStiffness;
//
//	btRaycastVehicle* vehicle = new btRaycastVehicle(tuning, body, vehicle_raycaster);
//
//	vehicle->setCoordinateSystem(0, 1, 2);
//
//	for(int i = 0; i < info.num_wheels; ++i)
//	{
//		btVector3 conn(info.wheels[i].connection.x, info.wheels[i].connection.y, info.wheels[i].connection.z);
//		btVector3 dir(info.wheels[i].direction.x, info.wheels[i].direction.y, info.wheels[i].direction.z);
//		btVector3 axis(info.wheels[i].axis.x, info.wheels[i].axis.y, info.wheels[i].axis.z);
//
//		vehicle->addWheel(conn, dir, axis, info.wheels[i].suspensionRestLength, info.wheels[i].radius, tuning, info.wheels[i].front);
//	}
//	// ---------------------
//
//	PhysVehicle3D* pvehicle = new PhysVehicle3D(body, vehicle, info);
//
//	world->addVehicle(vehicle);
//	body->setUserPointer(pvehicle);
//	vehicles.push_back(pvehicle);
//
//	return pvehicle;
//}

// ---------------------------------------------------------
btTypedConstraint* ModulePhysics3D::AddConstraintP2P(ComponentCollider& bodyA, ComponentCollider& bodyB, const vec3& anchorA, const vec3& anchorB)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.m_Body), 
		*(bodyB.m_Body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z), 
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	world->addConstraint(p2p);
	constraints.push_back(p2p);
	p2p->setDbgDrawSize(2.0f);

	return p2p;
}

btTypedConstraint* ModulePhysics3D::AddConstraintHinge(ComponentCollider& bodyA, ComponentCollider& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisA, const vec3& axisB, bool disable_collision)
{
	btTypedConstraint* hinge = new btHingeConstraint(
		*(bodyA.m_Body), 
		*(bodyB.m_Body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z),
		btVector3(axisA.x, axisA.y, axisA.z), 
		btVector3(axisB.x, axisB.y, axisB.z));

	world->addConstraint(hinge, disable_collision);
	constraints.push_back(hinge);
	hinge->setDbgDrawSize(2.0f);
	return hinge;
}

btTypedConstraint* ModulePhysics3D::AddConstraintSlider(ComponentCollider& bodyA, ComponentCollider& bodyB)
{
	btTypedConstraint* slider = new btSliderConstraint(
		*(bodyA.m_Body),
		*(bodyB.m_Body),
		bodyA.m_Body->getWorldTransform(),
		bodyB.m_Body->getWorldTransform(),
		false);

	world->addConstraint(slider);
	constraints.push_back(slider);
	slider->setDbgDrawSize(2.0f);
	return slider;
}

btTypedConstraint* ModulePhysics3D::AddConstraintSixDof(btRigidBody& bodyA, btRigidBody& bodyB, const btTransform& transform1, const btTransform& transform2)
{
	btGeneric6DofConstraint * free = new btGeneric6DofConstraint(bodyA, bodyB, transform1, transform2, true);
	for (int i = 1; i <= 6; ++i)
	{
		free->setLimit(i, 0, 0);
	}
	world->addConstraint(free,false);
	constraints.push_back(free);
	free->setDbgDrawSize(2.0f);
	bodyB.setDamping(100000, 100000);
	return free;
}


// =============================================

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	//line.origin.Set(from.getX(), from.getY(), from.getZ());
	//line.destination.Set(to.getX(), to.getY(), to.getZ());
	//line.color.Set(255, 0, 0);
	//line.Render();
	glUseProgram(0);
	glColor3f(0, 0, 255);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(from.getX(), from.getY(), from.getZ());
	glVertex3f(to.getX(), to.getY(), to.getZ());
	glEnd();
	glLineWidth(1.0f);
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
