#ifndef _ComponentMesh_H
#define _ComponentMesh_H
#include "Component.h"
#include "shader.h"

class Mesh;
using namespace std;
class ComponentMesh : public Component
{
public:
	ComponentMesh(Mesh* mesh);
	ComponentMesh();
	~ComponentMesh();

	void OnEditor();
	void SetMesh(Mesh* mesh);
	Mesh* GetMesh();

	bool m_DisplayNormals = false;
private:
	Mesh* m_Mesh = nullptr;

};
#endif