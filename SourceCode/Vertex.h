#ifndef __Vertex_H__
#define __Vertex_H__
#include "Color.h"
#include "MathGeoLib/MathGeoLib.h"
#include "glmath.h"
using namespace math;
//struct Vertex
//{
//	float3 pos;
//	float r, g, b;
//
//	Vertex() { r = g = b = 0.0f; };
//	Vertex(float x, float y, float z) {
//		pos = float3(x, y, z);
//		// Normalized values between 0 and 1
//		r = (float)rand() / (float)RAND_MAX;
//		g = (float)rand() / (float)RAND_MAX;
//		b = (float)rand() / (float)RAND_MAX;
//	}
//
//};

struct Vertex
{
	vec3 pos;
	vec3 color;

	Vertex() {};
	Vertex(float x, float y, float z) {
		pos = vec3(x, y, z);
		// Normalized values between 0 and 1
		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		color = vec3(r, g, b);
	}

};

#endif // 