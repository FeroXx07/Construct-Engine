#pragma once
#include "Module.h"
#include "Globals.h"

#include <list>

#include "shader.h"
#include "Model.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:
	bool hasCheckPoint = false;

protected:
	bool debug_draw = true;
	void BeginDebugDraw();
	void DebugDraw();
	void EndDebugDraw();

	uint VBO = 0, VAO = 0, EBO = 0;
	Shader* ourShader = nullptr;
	unsigned int texture1, texture2;

	float Scale = 0.0f;
	Model* ourModel = nullptr;
};
