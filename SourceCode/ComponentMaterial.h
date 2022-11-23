#ifndef _ComponentMaterial_H
#define _ComponentMaterial_H
#include "Component.h"
#include <string>
#include "Mesh.h"
using namespace std;
class ComponentMaterial: public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	void PassTextures(vector<Texture> &t);
	void OnEditor();
	//vector<Texture> m_Textures;
	std::vector<Texture> m_Textures;
	bool m_DisplayChecker_Tex = false;
	string m_MaterialName = "No_Name";
};
#endif