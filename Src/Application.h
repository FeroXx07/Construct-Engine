#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUI.h"

#include <string>
#include <list>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

// for convenience
using json = nlohmann::json;
// Return variable name as str
#define getName(var)  #var
// Return variable name as str and save it in another string
#define getNameS(var, str)  sprintf(str, "%s", #var) 

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* uiManager;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(std::string url);
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};