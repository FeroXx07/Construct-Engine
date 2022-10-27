#ifndef __ModuleComponentsSystems_H__
#define __ModuleComponentsSystems_H__
#include "Module.h"
#include "Globals.h"
#include "ComponentMesh.h"
#include <vector>

class GameObject;
class ModuleComponentSys : public Module
{
public:
	ModuleComponentSys(Application* app, bool start_enabled = true);
	~ModuleComponentSys();

	bool Start();
	update_status Update(float dt);
	//void DrawGameObject(Shader& shader, GameObject* rootNode, bool dirty = false);

	void DrawGameObject(Shader& shader, GameObject* rootNode, glm::mat4x4 world);
	void DrawGameObject(Shader& shader, GameObject* rootNode);
	void DrawNormals(ComponentMesh* mesh, ComponentTransform* transform);
	bool CleanUp();

	//MeshSystem* m_MeshSystem = nullptr;
};
#endif