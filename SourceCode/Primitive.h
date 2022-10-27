//
//#pragma once
//#include "glmath.h"
//#include "Color.h"
//
//namespace myPrimitive {
//	class Primitive;
//	class Cube;
//	class Sphere;
//	class Cylinder;
//	class Line;
//	class Plane;
//}
//
//enum PrimitiveTypes
//{
//	Primitive_Point,
//	Primitive_Line,
//	Primitive_Plane,
//	Primitive_Cube,
//	Primitive_Sphere,
//	Primitive_Cylinder
//};
//
//class myPrimitive::Primitive
//{
//public:
//
//	Primitive();
//
//	void Update();
//	virtual void	Render() const;
//	virtual void	InnerRender() const;
//	void			SetPos(float x, float y, float z);
//	void			SetRotation(float angle, const vec3 &u);
//	void			Scale(float x, float y, float z);
//	PrimitiveTypes	GetType() const;
//
//public:
//	
//	Color color;
//	mat4x4 transform;
//	bool axis,wire;
//
//protected:
//	PrimitiveTypes type;
//};
//
//// ============================================
//class myPrimitive::Cube : public myPrimitive::Primitive
//{
//public :
//	Cube();
//	Cube(float sizeX, float sizeY, float sizeZ);
//	void InnerRender() const;
//public:
//	vec3 size;
//};
//
//// ============================================
//class myPrimitive::Sphere : public myPrimitive::Primitive
//{
//public:
//	Sphere();
//	Sphere(float radius);
//	void InnerRender() const;
//public:
//	float radius;
//};
//
//// ============================================
//class myPrimitive::Cylinder : public myPrimitive::Primitive
//{
//public:
//	Cylinder();
//	Cylinder(float radius, float height);
//	void InnerRender() const;
//public:
//	float radius;
//	float height;
//};
//
//// ============================================
//class myPrimitive::Line : public myPrimitive::Primitive
//{
//public:
//	Line();
//	Line(float x, float y, float z);
//	void InnerRender() const;
//public:
//	vec3 origin;
//	vec3 destination;
//};
//
//// ============================================
//class myPrimitive::Plane : public myPrimitive::Primitive
//{
//public:
//	Plane();
//	Plane(float x, float y, float z, float d);
//	void InnerRender() const;
//public:
//	vec3 normal;
//	float constant;
//};