
#pragma once
#include "Color.h"

#include <glm/glm.hpp>
struct Light
{
	Light();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	Color ambient;
	Color diffuse;
	glm::vec3 position;

	int ref;
	bool on;
};