#pragma once
#include "Module.h"
#include "Globals.h"
#include "Camera.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	std::vector<Camera*> cameras;
	Camera* currentCamera = nullptr;
	float lastX = 0;
	float lastY = 0;
	float lastScroll = 0;
	bool firstMouse = true;
	bool firstMouseScroll = true;
};