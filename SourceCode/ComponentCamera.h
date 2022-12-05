#ifndef _ComponentCamera_H
#define _ComponentCamera_H
#include "Component.h"
#include <string>
#include "Camera.h"
#include "Mesh.h"
using namespace std;
class ComponentCamera : public Component
{
public:
	ComponentCamera(const char* name, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	ComponentCamera(const char* name, Camera* camera);
	~ComponentCamera();
	
	void GenerateBuffers(Shader* m_ScreenShader = nullptr);
	void Update();
	void OnEditor();
	void RenderWindow();
	
	Camera* m_Camera = nullptr;
	Shader* m_ScreenShader = nullptr;
	string m_Name = "No_Name";

	// Frame Buffer config
	GLuint m_Framebuffer;
	// create a color attachment texture
	GLuint m_TextureColorbuffer;
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint m_Rbo;

	void BindFrameBuffer();

	float m_QuadVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int m_QuadVAO, m_QuadVBO;
};

#endif