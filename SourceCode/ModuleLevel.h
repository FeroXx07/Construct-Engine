#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include <list>


class ModuleLevel : public Module
{
public:
	ModuleLevel(Application* app, bool start_enabled = true);
	~ModuleLevel();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


public:
	myPrimitive::Plane p = myPrimitive::Plane(0, 1, 0, 0);

	bool hasCheckPoint = false;
	std::list<myPrimitive::Primitive*> primitives;

protected:
	bool debug_draw = true;
	void BeginDebugDraw();
	void DebugDraw();
	void EndDebugDraw();

	unsigned int vertexShader = 0;
	unsigned int fragmentShader = 0;
	unsigned int shaderProgram = 0;

	float vertices[12]= 
	{
		0.5f, 0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f, 0.5f, 0.0f   // top left 
	};

	unsigned int indices[6] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VBO = 0, VAO = 0, EBO = 0;
};
