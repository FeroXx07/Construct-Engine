#include "Component.h"
#include "GameObject.h"

Component::Component(ComponentType type_)
{
	m_Type = type_;
	m_GameObject = nullptr;
	m_isActive = true;
}

Component::Component()
{
	
}

Component::~Component()
{
	m_GameObject = nullptr;
}

const GameObject& Component::GetGameObject() const
{
	return	*m_GameObject;
}

GameObject& Component::GetGameObject()
{
	return	*m_GameObject;
}

void Component::SetGameObject(GameObject& go)
{
	m_hasGameObject = true;
	m_GameObject = &go;
}


void Component::Start()
{
}

void Component::Enable()
{
	m_isActive = true;
}

void Component::Update()
{
}

void Component::Disable()
{
	m_isActive = false;
}

void Component::OnEditor()
{
}
