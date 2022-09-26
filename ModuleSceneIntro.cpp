#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	Cube * ramp = new Cube(15, 0.1f, 15);
	ramp->SetRotation(-25, { 1.0f,0.0f,0.0f });
	ramp->color.Set(100, 100, 0, 0);
	primitives.add(ramp);

	for (uint n = 0; n < primitives.count(); n++)
	{
		Primitive *current;
		primitives.at(n, current);
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	p.color.Set(255, 255, 255, 0);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		Sphere* s = new Sphere(1);
		s->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
		float force = 30.0f;
		primitives.add(s);
	}

	for (uint n = 0; n < primitives.count(); n++)
	{
		Primitive* current;
		primitives.at(n, current);
		current->Update();
		current->Render();
	}

	return UPDATE_CONTINUE;
}

