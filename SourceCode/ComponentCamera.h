#ifndef _ComponentCamera_H
#define _ComponentCamera_H
#include "Component.h"
#include <string>
#include "Camera.h"
using namespace std;
class ComponentCamera : public Component
{
public:
	ComponentCamera(const char* name, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	ComponentCamera(const char* name, Camera* camera);
	~ComponentCamera();
	
	void Update();
	void OnEditor();
	Camera* camera = nullptr;
	string m_Name = "No_Name";
};

#endif