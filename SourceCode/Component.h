#ifndef _COMPONENT_H
#define _COMPONENT_H

class GameObject;

enum class Type
{
	TRANSFORM,
	MESH,
	MATERIAL
};

class Component
{
public:
	Component(Type type_, GameObject* go_);
	~Component();

	Type type;
	bool active = false;
	GameObject* owner = nullptr;

protected:
	virtual void Enable();
	virtual void Update();
	virtual void Disable();
};

#endif