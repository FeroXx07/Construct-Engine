#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

Component* GameObject::CreateComponent(Type type_)
{
	Component* cmp = new Component(type_, this);
	return cmp;
}
