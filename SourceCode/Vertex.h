#ifndef __Vertex_H__
#define __Vertex_H__
#include "Color.h"
#include "MathGeoLib/MathGeoLib.h"

using namespace math;
struct Vertex
{
	float3 pos;
	Color color;

	Vertex() {};
	Vertex(float x, float y, float z = 0) {
		pos = float3(x, y, z);
		// Normalized values between 0 and 1
		color.r = (float)rand() / (float)RAND_MAX;
		color.b = (float)rand() / (float)RAND_MAX;
		color.g = (float)rand() / (float)RAND_MAX;
	}

};

#endif // 