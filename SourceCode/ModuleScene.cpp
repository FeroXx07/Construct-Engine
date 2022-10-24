#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	ourShader = nullptr;
	ourModel = nullptr;
	srand(_getpid());
	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(vec3(0, 0, 0));

	//Vertex Vertices[8];
	//Vertices[0] = Vertex(0.5f, 0.5f, 0.5f);   // FRONT TOP RIGHT
	//Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f); // REAR TOP LEFT
	//Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f);  // FRONT TOP LEFT
	//Vertices[3] = Vertex(0.5f, -0.5f, -0.5f); // REAR BOTTOM RIGHT
	//Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f);// REAR BOTTOM LEFT 
	//Vertices[5] = Vertex(0.5f, 0.5f, -0.5f);  // REAR TOP RIGHT
	//Vertices[6] = Vertex(0.5f, -0.5f, 0.5f);  // FRONT BOTTOM RIGHT
	//Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f); // FRONT BOTTOM LEFT

	//unsigned int Indices[] = { 0, 2, 7,
	//						   1, 3, 4,
	//						    5, 6, 3,
	//						   7, 3, 6,
	//						   2, 4, 7,
	//						   0, 7, 6,
	//						   0, 5, 1,
	//						   1, 5, 3,
	//						   5, 0, 6,
	//						   7, 4, 3,
	//						   2, 1, 4,
	//						   0, 2, 7
	//};

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(false);
	ourShader = new Shader("Resources/Shaders/model_loading.vert", "Resources/Shaders/model_loading.frag"); // you can name your shader files however you like
	//ourShader = new Shader("Resources/Shaders/simple.vert", "Resources/Shaders/simple.frag"); // you can name your shader files however you like
	
	//ourModel = new Model("Resources/backpack/backpack.obj");
	//ourModel = new Model("Resources/Meshes/BirdFountain.fbx");
	ourModel = new Model("Resources/Meshes/omozra.fbx");
	////// set up vertex data (and buffer(s)) and configure vertex attributes

	//glGenVertexArrays(1, (GLuint*)&VAO);
	//glGenBuffers(1, (GLuint*)&VBO);
	//glGenBuffers(1, (GLuint*)&EBO);

	////// 1. bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//glBindVertexArray(VAO);

	////// 2. copy our vertices array in a vertex buffer for OpenGL to use
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	////// 3. copy our index array in a element buffer for OpenGL to use
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	//// 4 now that we have the data in the buffer (vram), the gpu needs to know how to interpret that data
	////// 4.1 then set the vertex attributes pointers for the POSITION (remember 3 floats X,Y,Z)		
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	////// 4.2 then set the vertex attributes pointers for the COLOR (remember 4 floats R,G,B,A)
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // stride (dist from x1 to x2) of 7 because -- X1-Y1-Z1-R1-G1-B1-A1-X2-Y2-Z2-R2-G2-B2-A1
	//glEnableVertexAttribArray(1);

	////// 4.3 texture coord attribute
	////glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
 ////   glEnableVertexAttribArray(1);

	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	debug_draw = false;

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	if (ourShader != nullptr)
		delete ourShader;

	if (ourModel != nullptr)
		delete ourModel;
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	return true;
}

void ModuleScene::BeginDebugDraw()
{
}

void ModuleScene::DebugDraw()
{
}

void ModuleScene::EndDebugDraw()
{
}

// Update
update_status ModuleScene::Update(float dt)
{

	Scale += 0.2f;
	
	int height, width;
	App->window->GetScreenSize(width, height);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(Scale), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	ourShader->setMat4("model", model);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(App->camera->currentCamera->Zoom), (float)width / (float)height, 0.1f, 100.0f);
	ourShader->setMat4("projection", projection);

	glm::mat4 view = App->camera->currentCamera->GetViewMatrix();
	ourShader->setMat4("view", view);

	
	// note that we're translating the scene in the reverse direction of where we want to move
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	//glm::mat4x4 trans = projection * view * model;
	//ourShader->setMat4("trans", trans);
	
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	if (debug_draw == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	// bind textures on corresponding texture units
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture2);
	ourShader->use();
	ourModel->Draw(*ourShader);
	//ourShader->use();
	/*glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/
	return UPDATE_CONTINUE;
}

