#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "PhysBody3D.h"

#include <string>
#include <vector>
#include "Json.h"
using namespace std;
class GameObject
{
public:
	int id;
	GameObject();
	GameObject(string name);
	~GameObject();
	
	Component* GetComponent(ComponentType type);

	//	Finds all components of given type in the whole	scene (parent and children)
	vector<Component*> FindAllComponentsOfType(ComponentType type);
	GameObject* FindById(int id);

	void SetPhysBody(PhysBody3D* newBody);
	void SetParent(GameObject* parent);
	void SetChild(GameObject* child);

	PhysBody3D* GetPhysBody();
	GameObject* GetParent();
	GameObject* GetParentConst() const;

	void UpdateBody();

	void AddChild(GameObject* newChild);
	void RemoveChild(GameObject* child);
	bool IsGameObjectChild(GameObject* possibleChild);

	string m_Name = " ";
	vector<GameObject*> m_Children;
	OBB m_Obb;
	AABB m_Aabb;
	void GenerateBoundingBoxes();
	PhysBody3D* m_PhysBody = nullptr;
protected:
	/*vector<Component*> m_Components;*/
	GameObject* m_Parent = nullptr;

	// data for all components
	ComponentMesh* m_ComponentMesh;
	ComponentTransform* m_ComponentTransform;
	ComponentMaterial* m_ComponentMaterial;
	ComponentCamera* m_ComponentCamera;
	static std::atomic<int> s_id;

public:
	// flags for every component, indicating whether this object "has it"
	bool m_HasComponentMesh = false;
	bool m_HasComponentTransform = false;
	bool m_HasComponentMaterial= false;
	bool m_HasComponentCamera= false;

	// Components assignators
	void AssignComponent(ComponentMesh* comp);
	void AssignComponent(ComponentTransform* comp);
	void AssignComponent(ComponentMaterial* comp);
	void AssignComponent(ComponentCamera* comp);

	ComponentMesh* GetMesh();
	ComponentTransform* GetTransform();
	ComponentMaterial* GetMaterial();
	ComponentCamera* GetCamera();

	ComponentMesh* GetMeshConst() const;
	ComponentTransform* GetTransformConst() const;
	ComponentMaterial* GetMaterialConst() const;
	ComponentCamera* GetCameraConst() const;

public:
	bool gui_IsSelected = false;


};

#endif


