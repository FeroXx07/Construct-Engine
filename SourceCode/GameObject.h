#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include <string>
#include <vector>

using namespace std;
class GameObject
{
public:
	GameObject();
	GameObject(string name);
	~GameObject();
	
	Component* GetComponent(ComponentType type);

	//	Finds all components of given type in the whole	scene (parent and children)
	vector<Component*> FindAllComponentsOfType(ComponentType type);

	void SetParentAndChild(GameObject* parent);
	GameObject* GetParent();

	void AddChild(GameObject* newChild);
	bool IsGameObjectChild(GameObject* possibleChild);

	string m_Name = " ";
	vector<GameObject*> m_Children;
protected:
	/*vector<Component*> m_Components;*/
	GameObject* m_Parent = nullptr;

	// data for all components
	ComponentMesh* m_ComponentMesh;
	ComponentTransform* m_ComponentTransform;

public:
	// flags for every component, indicating whether this object "has it"
	bool m_HasComponentMesh = false;
	bool m_HasComponentTransform = false;

	// Components assignators
	void AssignComponent(ComponentMesh* comp);
	void AssignComponent(ComponentTransform* comp);
	ComponentMesh* GetMesh();
	ComponentTransform* GetTransform();

public:
	bool gui_IsSelected = false;
};

#endif


