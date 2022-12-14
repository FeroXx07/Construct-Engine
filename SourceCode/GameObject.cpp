#include "GameObject.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
std::atomic<int> GameObject::s_id;

GameObject::GameObject() : id(++s_id), m_Parent(nullptr), m_ComponentMesh(nullptr), m_ComponentTransform(nullptr), m_ComponentMaterial(nullptr), m_ComponentCamera(nullptr), m_ComponentCollider(nullptr), m_ComponentConstraint(nullptr), 
m_HasComponentMesh(0), m_HasComponentTransform(0), m_HasComponentMaterial(0), m_HasComponentCamera(0), m_HasComponentCollider(0), m_HasComponentConstraint(0)
{
	m_Name = "No Name!!";
}

GameObject::GameObject(string name) :id(++s_id), m_Name(name), m_Parent(nullptr), m_ComponentMesh(nullptr), m_ComponentTransform(nullptr), m_ComponentMaterial(nullptr), m_ComponentCamera(nullptr), m_ComponentCollider(nullptr), m_ComponentConstraint(nullptr),
m_HasComponentMesh(0), m_HasComponentTransform(0), m_HasComponentMaterial(0), m_HasComponentCamera(0), m_HasComponentCollider(0), m_HasComponentConstraint(0)
{
}


GameObject::~GameObject()
{
	if (this->m_Parent != nullptr)
	{
		m_Parent->RemoveChild(this);
	}
	m_Parent = nullptr;

	int size = m_Children.size();
	for (int i = size - 1; i >= 0; i--)
	{
		delete m_Children[i];
	}

	// Delete mesh component
	if (m_ComponentMesh != nullptr)
	{
		delete m_ComponentMesh->GetMesh();
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

	// Delete material component
	if (m_ComponentCamera != nullptr)
	{
		delete m_ComponentCamera;
		m_ComponentCamera = nullptr;
	}

	// Delete collider component
	if (m_ComponentCollider != nullptr)
	{
		delete m_ComponentCollider;
		m_ComponentCollider = nullptr;
	}

	//// Delete constraint component
	//if (m_ComponentConstraint != nullptr)
	//{
	//	delete m_ComponentConstraint;
	//	m_ComponentConstraint = nullptr;
	//}
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
	{
		if (m_HasComponentMaterial) { return m_ComponentMaterial; };
	}
		break;
	case ComponentType::CAMERA:
	{
		if (m_HasComponentMaterial) { return m_ComponentMaterial; };
	}
	case ComponentType::COLLIDER:
	{
		if (m_HasComponentCollider) { return m_ComponentCollider; };
	}
	case ComponentType::CONSTRAINT:
	{
		if (m_HasComponentConstraint) { return m_ComponentConstraint; };
	}
	break;
	default:
		return nullptr;
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

GameObject* GameObject::FindById(int id)
{
	if (this->id == id)
	{
		return this;
	}
	for (auto chl : this->m_Children)
	{
		if (chl->id == id)
		{
			return chl;
		}
		chl->FindById(id);
	}
	return nullptr;
}

GameObject* GameObject::FindByName(string name)
{
	if (this->m_Name == name)
	{
		return this;
	}
	for (auto chl : this->m_Children)
	{
		if (chl->m_Name == name)
		{
			return chl;
		}
		chl->FindByName(name);
	}
	return nullptr;
}

//void GameObject::SetPhysBody(PhysBody3D* newBody)
//{
//	m_PhysBody = newBody;
//	m_PhysBody->SetGameObject(this);
//	m_PhysBody->m_Body->setGravity({0,0,0});
//}

void GameObject::SetParent(GameObject* parent)
{
	if (m_Parent != nullptr)
	{
		// Remove himself from children list of current parent
		m_Parent->RemoveChild(this);
	}
	// Set new parent
	m_Parent = parent;
}

void GameObject::SetChild(GameObject* child)
{
	// Add himself as new child to the children list of the new parent
	AddChild(child);
}

GameObject* GameObject::GetParentConst()const
{
	return m_Parent;
}

void GameObject::UpdateBody()
{
	/*if (m_PhysBody != nullptr)
	{
		m_PhysBody->GetTransform(m_ComponentTransform->m_LocalMat);
	}*/
}

GameObject* GameObject::GetParent()
{
	return m_Parent;
}

void GameObject::AddChild(GameObject* newChild)
{
	if (IsGameObjectChild(newChild) == false)
		m_Children.push_back(newChild);
}

void GameObject::RemoveChild(GameObject* child)
{
	uint i = 0;
	for (auto go : m_Children)
	{
		if (go->id == child->id)
			m_Children.erase(m_Children.begin() + i);
		++i;
	}
}

bool GameObject::IsGameObjectChild(GameObject* possibleChild)
{
	bool isChildren = false;
	for (auto go : m_Children)
		if (go == possibleChild)
			isChildren = true;
	return isChildren;
}

void GameObject::GenerateBoundingBoxes()
{
	if (m_HasComponentMesh && m_HasComponentTransform)
	{
		// Generate global OBB
		m_Obb = GetMeshConst()->GetMesh()->GetAABB();
		mat4x4 transformMatrix = GetTransformConst()->GetWorld();
		math::float4x4 a;
		a = a.FromGLM(transformMatrix);
		m_Obb.Transform(a);
		// Generate global AABB
		m_Aabb.SetNegativeInfinity();
		m_Aabb.Enclose(m_Obb);
	}
}

void GameObject::AssignComponent(ComponentMesh* comp)
{
	m_ComponentMesh = comp;
	m_HasComponentMesh = true;
	comp->SetGameObject(*this);
	GenerateBoundingBoxes();
}

void GameObject::AssignComponent(ComponentTransform* comp)
{
	m_ComponentTransform = comp;
	m_HasComponentTransform = true;
	comp->m_Dirty = true;
	comp->SetGameObject(*this);
}

void GameObject::AssignComponent(ComponentMaterial* comp)
{
	m_ComponentMaterial = comp;
	m_HasComponentMaterial = true;
	comp->SetGameObject(*this);
}

void GameObject::AssignComponent(ComponentCamera* comp)
{
	m_ComponentCamera = comp;
	m_HasComponentCamera = true;
	comp->SetGameObject(*this);
}

void GameObject::AssignComponent(ComponentCollider* comp)
{
	m_ComponentCollider = comp;
	m_HasComponentCollider = true;
	comp->SetGameObject(*this);
}

void GameObject::AssignComponent(ComponentConstraint* comp)
{
	m_ComponentConstraint = comp;
	m_HasComponentConstraint = true;
	comp->SetGameObject(*this);
}

void GameObject::DeAssignComponent(ComponentType type)
{
	switch (type)
	{
	case ComponentType::TRANSFORM:
	{
		m_ComponentTransform = nullptr;
		m_HasComponentTransform = false;
		break;
	}
	case ComponentType::MESH:
	{
		m_ComponentMesh = nullptr;
		m_HasComponentMesh = false;
		break;
	}
	case ComponentType::MATERIAL:
	{
		m_ComponentMaterial = nullptr;
		m_HasComponentMaterial= false;
		break;
	}
	case ComponentType::CAMERA:
	{
		m_ComponentCamera = nullptr;
		m_HasComponentCamera = false;
		break;
	}
	case ComponentType::COLLIDER:
	{
		m_ComponentCollider = nullptr;
		m_HasComponentCollider = false;
		break;
	}
	case ComponentType::CONSTRAINT:
	{
		m_ComponentConstraint = nullptr;
		m_HasComponentConstraint = false;
		break;
	}
	default:
		break;
	}
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

ComponentCamera* GameObject::GetCamera()
{
	if (m_HasComponentCamera)
		return m_ComponentCamera;
	else
		return nullptr;
}

ComponentCollider* GameObject::GetCollider()
{
	if (m_HasComponentCollider)
		return m_ComponentCollider;
	else
		return nullptr;
}

ComponentConstraint* GameObject::GetConstraint()
{
	if (m_HasComponentConstraint)
		return m_ComponentConstraint;
	else
		return nullptr;
}

ComponentMesh* GameObject::GetMeshConst() const
{
	if (m_HasComponentMesh)
		return m_ComponentMesh;
	else
		return nullptr;
}

ComponentTransform* GameObject::GetTransformConst() const
{
	if (m_HasComponentTransform)
		return m_ComponentTransform;
	else
		return nullptr;
}

ComponentMaterial* GameObject::GetMaterialConst() const
{
	if (m_HasComponentMaterial)
		return m_ComponentMaterial;
	else
		return nullptr;
}

ComponentCamera* GameObject::GetCameraConst() const
{
	if (m_HasComponentCamera)
		return m_ComponentCamera;
	else
		return nullptr;
}

ComponentCollider* GameObject::GetColliderConst() const
{
	if (m_HasComponentCollider)
		return m_ComponentCollider;
	else
		return nullptr;
}

ComponentConstraint* GameObject::GetConstraintConst() const
{
	if (m_HasComponentConstraint)
		return m_ComponentConstraint;
	else
		return nullptr;
}
