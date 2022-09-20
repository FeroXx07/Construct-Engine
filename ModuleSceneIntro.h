#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "p2List.h"
#define MAX_SNAKE 2

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


public:
	Plane p = Plane(0, 1, 0, 0);

	Cube* checkPoint = nullptr;
	Cube* deathSensor = nullptr;
	Cube* impulseSensor = nullptr;
	Cube* winSensor = nullptr;

	bool hasCheckPoint = false;
	p2List<Primitive*> primitives;
};
