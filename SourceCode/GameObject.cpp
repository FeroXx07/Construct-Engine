#include "GameObject.h"

GameObject::GameObject() : m_Parent(nullptr), m_ComponentMesh(nullptr), m_ComponentTransform(nullptr), m_ComponentMaterial(nullptr), m_HasComponentMesh(0), m_HasComponentTransform(0)
{
	m_Name = "No Name!!";
}

GameObject::GameObject(string name) : m_Name(name), m_Parent(nullptr), m_ComponentMesh(nullptr), m_ComponentTransform(nullptr), m_ComponentMaterial(nullptr), m_HasComponentMesh(0), m_HasComponentTransform(0)
{
}


GameObject::~GameObject()
{
	m_Parent = nullptr;

	// Delete mesh component
	if (m_ComponentMesh != nullptr)
	{
		delete m_ComponentMesh;
		m_ComponentMesh = nullptr;
	}
	
	// Delete transform component
	if (m_ComponentTransform != nullptr)
	{
		delete m_ComponentTransform;
		m_ComponentTransform = nullptr;
	}
	
	// Delete material component
	if (m_ComponentMaterial != nullptr)
	{
		delete m_ComponentMaterial;
		m_ComponentMaterial = nullptr;
	}
}

Component* GameObject::GetComponent(ComponentType type)
{
	switch (type)
	{
	case ComponentType::TRANSFORM:
	{
		if (m_HasComponentTransform) { return m_ComponentTransform; };
	}
		break;
	case ComponentType::MESH:
	{
		if (m_HasComponentMesh) { return m_ComponentMesh; };
	}
		break;
	case ComponentType::MATERIAL:
		break;
	default:
		break;
	}
}

vector<Component*> GameObject::FindAllComponentsOfType(ComponentType type)
{
	ComponentVector res;
	for (auto go : m_Children)
	{
		Component* c = go->GetComponent(type);
		if (c != nullptr)	res.emplace_back(c);
	}
	return res;
}

void GameObject::SetParentAndChild(GameObject* parent)
{
	if (parent != nullptr)
	{
		m_Parent = parent;
		m_Parent->AddChild(this);
	}
}

GameObject* GameObject::GetParent()
{
	return m_Parent;
}

void GameObject::AddChild(GameObject* newChild)
{
	if (IsGameObjectChild(this) == false)
		m_Children.emplace_back(newChild);
}

bool GameObject::IsGameObjectChild(GameObject* possibleChild)
{
	bool isChildren = false;
	for (auto go : m_Children)
		if (go == possibleChild)
			isChildren = true;
	return isChildren;
}

void GameObject::AssignComponent(ComponentMesh* comp)
{
	m_ComponentMesh = comp;
	m_HasComponentMesh = true;
}

void GameObject::AssignComponent(ComponentTransform* comp)
{
	m_ComponentTransform = comp;
	m_HasComponentTransform = true;
	comp->m_Dirty = true;
}

void GameObject::AssignComponent(ComponentMaterial* comp)
{
	m_ComponentMaterial = comp;
	m_HasComponentMaterial = true;
}

ComponentMesh* GameObject::GetMesh()
{
	if (m_HasComponentMesh)
		return m_ComponentMesh;
	else
		return nullptr;
}

ComponentTransform* GameObject::GetTransform()
{
	if (m_HasComponentTransform)
		return m_ComponentTransform;
	else
		return nullptr;
}

ComponentMaterial* GameObject::GetMaterial()
{
	if (m_HasComponentMaterial)
		return m_ComponentMaterial;
	else
		return nullptr;
}

