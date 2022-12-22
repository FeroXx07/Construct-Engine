//#ifndef __PhysBody3D_H__
//#define __PhysBody3D_H__
//
//#include <list>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//class btRigidBody;
//class Module;
//class GameObject;
////class Sphere;
////class Cube;
////class Cylinder;
////class Plane;
////
////class Primitive;
//
//enum Shape {
//	SPHERE,
//	CUBE,
//	CYLINDER
//};
//// =================================================
//struct PhysBody3D
//{
//	friend class ModulePhysics3D;
//	friend class ModulePlayer;
//	friend class ModuleSceneIntro;
//public:
//	PhysBody3D(btRigidBody* body);
//	~PhysBody3D();
//
//	void Push(float x, float y, float z);
//	void GetTransform(glm::mat4& mat) const;
//	void SetTransform(glm::mat4& mat) const;
//	void SetPos(float x, float y, float z);
//
//	void SetGameObject(GameObject* ownerGameObject);
//	void PhysBody3D::SetAsSensor(bool is_sensor);
//	
//	btRigidBody* GetBody()
//	{
//		return m_Body;
//	}
//	Shape shape = Shape::CUBE;
//	/*Primitive* parentPrimitive;*/
//
//
//	btRigidBody* m_Body = nullptr;
//	GameObject* ownerGameObject = nullptr;
//public:
//	std::list<Module*> collision_listeners;
//	bool is_sensor = false;
//
//	//glm::mat4x4 BtToGLM(btScalar* m, glm::mat4x4& dst)
//	//{
//	//	// convert to (column major)
//	//	dst[0][0] = m[0];
//	//	dst[0][0] = m[0];
//	//	dst[0][0] = m[0];
//	//	dst[0][0] = m[0];
//
//	//}
//};
//
//#endif // __PhysBody3D_H__