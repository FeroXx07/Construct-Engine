#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include <list>
#include "shader.h"
#include "MathGeoLib/MathGeoLib.h"

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

	uint VBO = 0, VAO = 0, EBO = 0;
	Shader* ourShader = nullptr;
	GLint gScaleLocation;
	GLint gTransLocation;
	vec3 Vertices[3];

	float Scale = 0.0f;
	float Delta = 0.005f;
	
	math::float4x4 translation;
};
