#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (auto item = list_modules.rbegin(); item != list_modules.rend(); ++item)
	{
		if (*item != NULL)
		{
			delete (*item);
		}
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (auto item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		if (*item != NULL && ret == true)
			ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (auto item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		if (*item != NULL && ret == true)
			ret = (*item)->Start();
	}
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (auto item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		if (*item != NULL && ret == true)
			ret = (*item)->PreUpdate(dt);
	}
	for (auto item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		if (*item != NULL && ret == true)
			ret = (*item)->Update(dt);
	}
	for (auto item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		if (*item != NULL && ret == true)
			ret = (*item)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (auto item = list_modules.rbegin(); item != list_modules.rend(); ++item)
	{
		if (*item != NULL && ret == true)
			ret = (*item)->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}