#pragma once
#include "Module.h"
#include "Globals.h"

#include "shader.h"
#include "ModelLoader.h"
#include "GameObject.h"

#include <list>
#include "Primitive.h"
class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(string const& path, string name);
	GameObject* CreateEmptyGameObject(string name);
	GameObject* CreateCamera(string name, GameObject* destinationGO = nullptr);
	
	GameObject* root = nullptr;

	bool debug_draw = true;
	Shader* modelsShader = nullptr;
	Shader* screenShader = nullptr;

	float Scale = 0.0f;
	ModelLoader* m_ModelLoader = nullptr;

	glm::mat4 projection;

	void SaveSceneJson();
	void LoadSceneJson();
	void DeleteScene();
	void To_Json(json& j, const GameObject* go);
	GameObject* From_Json(const json& j, const GameObject* goParent);

	P_Plane p = P_Plane(0, 1, 0, 0);
};
