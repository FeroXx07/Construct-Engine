#pragma once
#include "Module.h"
#include "Globals.h"
#include <list>
#include "shader.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Pipeline.h"
#include "Vertex.h"

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
	bool hasCheckPoint = false;

protected:
	bool debug_draw = true;
	void BeginDebugDraw();
	void DebugDraw();
	void EndDebugDraw();

	uint VBO = 0, VAO = 0, EBO = 0;
	Shader* ourShader = nullptr;
	unsigned int texture1, texture2;

	float Scale = 0.0f;
	
	math::float4x4 matrix;

	float4x4 perspective(float fovy, float aspect, float n, float f)
	{
		float4x4 Perspective = float4x4::identity;

		float coty = 1.0f / tan(fovy * (float)pi / 360.0f);

		Perspective[0][0] = coty / aspect;
		Perspective[1][1] = coty;
		Perspective[2][2] = (n + f) / (n - f);
		Perspective[2][3] = -1.0f;
		Perspective[3][2] = 2.0f * n * f / (n - f);
		Perspective[3][3] = 0.0f;

		return Perspective;
	}
};
