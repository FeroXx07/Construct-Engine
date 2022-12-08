#pragma once
#include "Module.h"
#include "Globals.h"
#include "ComponentCamera.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	std::vector<ComponentCamera*> cameras;
	ComponentCamera* editorCamera = nullptr;
	void ChangeCamerasAspectRatio(const int width, const int height);
	float lastX = 0;
	float lastY = 0;
	float lastScroll = 0;
	bool firstMouse = true;
	bool firstMouseScroll = true;
};