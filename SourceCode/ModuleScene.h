#pragma once
#include "Module.h"
#include "Globals.h"

#include "shader.h"
#include "ModelLoader.h"
#include "GameObject.h"

#include <list>

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
	
	GameObject* root = nullptr;

	bool debug_draw = true;
	Shader* ourShader = nullptr;
	float Scale = 0.0f;
	ModelLoader* m_ModelLoader = nullptr;

	glm::mat4 projection;
};
