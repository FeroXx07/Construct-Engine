#ifndef _COMPONENT_H
#define _COMPONENT_H
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
class GameObject;

struct GuiInputData
{
	bool m_OpHasBeenEdited = true;
	glm::vec3 data;
	void SetData(glm::vec3 newData)
	{
		if (m_OpHasBeenEdited)
		{
			data = newData;
			m_OpHasBeenEdited = false;
		}
			
	}
};

enum class ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
	COLLIDER
};

class Component
{
public:
	Component(ComponentType type_);
	Component();
	~Component();

	const GameObject& GetGameObject() const;
	GameObject& GetGameObject();
	// Called from AddComponent in GameObject class
	void SetGameObject(GameObject& go);
	void RemoveComponentFromGameObject();

	ComponentType GetType() const { return m_Type; }
	bool m_hasGameObject = false;
protected:
	ComponentType m_Type;
	bool m_isActive = false;
	GameObject* m_GameObject = nullptr;

protected:
	virtual void Start();
	virtual void Enable();
	virtual void Update();
	virtual void Disable();

public:
	inline bool GetIsActive() { return m_isActive; };
	virtual void OnEditor();
};
typedef std::vector<Component*> ComponentVector;
#endif