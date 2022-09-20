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

	// Z LEFT RIGHT  // X FORWARD REAR
	/*Cube* leftStart = new Cube(5, 2, 50);
	App->physics->AddBody(*leftStart, 10000)->SetPos(5, 0, 20);
	primitives.PushBack(leftStart);

	Cube *rightStart = new Cube(5, 2, 50);
	App->physics->AddBody(*rightStart, 10000)->SetPos(-5, 0, 20);
	primitives.PushBack(rightStart);*/

	Cube * ramp = new Cube(15, 0.1f, 15);
	ramp->SetRotation(-25, { 1.0f,0.0f,0.0f });
	ramp->color.Set(100, 100, 0, 0);
	primitives.add(ramp);

	Cube * platform1 = new Cube(15, 2, 40);
	platform1->color.Set(0, 100, 255, 0);
	primitives.add(platform1);

	Cube* wall1 = new Cube(15, 15, 2);
	wall1->color.Set(0, 0, 255, 0);
	primitives.add(wall1);

	//Cube* platform2 = new Cube(30, 2, 15);
	//platform2->color.Set(0, 100, 255, 0);
	//primitives.add(platform2);

	//Cube* ramp2 = new Cube(15, 0.1f, 15);
	//ramp2->SetRotation(-25, { 0.0f,0.0f,1.0f });
	//ramp2->color.Set(0, 0, 255, 0);
	//primitives.add(ramp2);

	//Cube* platform3 = new Cube(30, 2, 25);
	//platform3->color.Set(0, 100, 255, 0);
	//primitives.add(platform3);

	//Cube* wheelLineLeft = new Cube(1.5f, 2, 50);
	//wheelLineLeft->color.Set(100, 100, 0, 0);
	//primitives.add(wheelLineLeft);

	//Cube* wheelLineRight = new Cube(1.5f, 2, 50);
	//wheelLineRight->color.Set(100, 100, 0, 0);
	//primitives.add(wheelLineRight);

	//Cube* platform4 = new Cube(30, 2, 25);
	//platform4->color.Set(0, 100, 255, 0);
	//primitives.add(platform4);

	//Cube* wall2 = new Cube(2, 40, 30);
	//wall2->color.Set(0, 0, 255, 0);
	//primitives.add(wall2);

	//Cube* wall3 = new Cube(2, 40, 30);
	//wall3->color.Set(0, 0, 255, 0);
	//primitives.add(wall3);

	//Cube* platform5 = new Cube(30, 2, 30);
	//platform5->color.Set(0, 100, 255, 0);
	//primitives.add(platform5);

	//Cube* platform6 = new Cube(30, 2, 30);
	//platform6->color.Set(0, 100, 255, 0);
	//primitives.add(platform6);

	//Cube* platform7 = new Cube(30, 2, 30);
	//platform7->color.Set(0, 100, 255, 0);
	//primitives.add(platform7);

	//Cube* platform8 = new Cube(30, 2, 30);
	//platform8->color.Set(0, 100, 255, 0);
	//primitives.add(platform8);

	//Cube* platform9 = new Cube(30, 2, 30);
	//platform9->color.Set(0, 100, 255, 0);
	//primitives.add(platform9);

	//Cube* ramp3 = new Cube(32, 0.1f, 30);
	//ramp3->SetRotation(21, { 0.0f,0.0f,1.0f });
	//ramp3->color.Set(0, 0, 255, 0);
	//primitives.add(ramp3);

	//Cube* platform10 = new Cube(50, 2, 100);
	//platform10->color.Set(0, 100, 255, 0);
	//primitives.add(platform10);

	//checkPoint = new Cube(50, 1, 5);
	//checkPoint->color.Set(255, 0, 0, 0.2f);
	//primitives.add(checkPoint);

	//deathSensor = new Cube(300, 1, 1000);
	//deathSensor->color.Set(0, 0, 0, 0.2f);
	//primitives.add(deathSensor);

	//Cube* ground = new Cube(300, 1, 200);
	//ground->color.Set(255, 255, 255, 0.2f);
	//primitives.add(ground);

	//Cube* behindWall = new Cube(300, 500, 1);
	//behindWall->color.Set(255, 255, 0, 0.2f);
	//primitives.add(behindWall);

	//Cube* leftWall = new Cube(1, 500, 1000);
	//leftWall->color.Set(255, 255, 0, 0.2f);
	//primitives.add(leftWall);

	//Cube* rightWall = new Cube(1, 500, 1000);
	//rightWall->color.Set(255, 255, 0, 0.2f);
	//primitives.add(rightWall);

	//Cube* forwardWall = new Cube(300, 500, 1);
	//forwardWall->color.Set(255, 255, 0, 0.2f);
	//primitives.add(forwardWall);

	//// SECOND PART OF THE CIRCUIT
	//Cube* corridor = new Cube(10, 2, 200);
	//corridor->color.Set(0, 100, 255, 0);
	//primitives.add(corridor);

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

