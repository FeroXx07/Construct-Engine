#ifndef __Vertex_H__
#define __Vertex_H__
#include "Color.h"
#include "MathGeoLib/MathGeoLib.h"

using namespace math;
struct Vertex
{
	float3 pos;
	float r, g, b;
	float2 tex;

	Vertex() { r = g = b = 0.0f; };
	Vertex(float x, float y, float z, float tx = 0, float ty = 0) {
		pos = float3(x, y, z);
		tex = float2(tx, ty);
		// Normalized values between 0 and 1
		r = (float)rand() / (float)RAND_MAX;
		g = (float)rand() / (float)RAND_MAX;
		b = (float)rand() / (float)RAND_MAX;
	}
	Vertex(float x, float y, float z, float r, float g, float b, float tx = 0, float ty = 0) {
		pos = float3(x, y, z);
		tex = float2(tx, ty);
		this->r = r;
		this->b = g;
		this->g = b;
	}

};

#endif // 