#include "Component.h"

Component::Component(Type type_, GameObject* go_) 
{
	type = type_;
	owner = go_;
	active = true;
}

Component::~Component()
{
}

void Component::Enable()
{
}

void Component::Update()
{
}

void Component::Disable()
{
}
