#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include "Component.h"
#include <string>
#include <vector>
using namespace std;
class GameObject
{
public:
	GameObject();
	~GameObject();
		
	Component* CreateComponent(Type type_);

	string name = " ";
	vector<Component> components;
	GameObject* parent = nullptr;
	vector<GameObject*> children;
};

#endif