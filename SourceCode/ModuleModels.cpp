#include "Application.h"
#include "ModuleModels.h"

ModuleModels::ModuleModels(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleModels::~ModuleModels()
{
}

bool ModuleModels::Start()
{
	return true;
}

update_status ModuleModels::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleModels::CleanUp()
{
	return true;
}
