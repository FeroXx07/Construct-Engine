#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleComponentSys.h"

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "ComponentMesh.h"
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
	m_ModelLoader = nullptr;

	srand(_getpid());

	root = new GameObject("RootNode");
	ComponentTransform* rootTrans = new ComponentTransform();
	root->AssignComponent(rootTrans);
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);
	ourShader = new Shader("Resources/Shaders/model_loading.vert", "Resources/Shaders/model_loading.frag"); // you can name your shader files however you like
	//ourShader = new Shader("Resources/Shaders/simple.vert", "Resources/Shaders/simple.frag"); // you can name your shader files however you like
	
	//ourModel = new Model("Resources/backpack/backpack.obj");
	//ourModel = new Model("Resources/Meshes/BirdFountain.fbx");
	m_ModelLoader = new ModelLoader(App->componentsManager);
	//m_ModelLoader->LoadModelFrom_aiScene("Resources/Meshes/omozra.fbx", this->root);
	m_ModelLoader->LoadModelFrom_aiScene("Resources/Meshes/BakerHouse.fbx", this->root);

	debug_draw = false;

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	if (ourShader != nullptr)
		delete ourShader;

	if (m_ModelLoader != nullptr)
		delete m_ModelLoader;
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	return true;
}

GameObject* ModuleScene::CreateGameObject()
{
	return nullptr;
}

// Update
update_status ModuleScene::Update(float dt)
{

	Scale += 0.2f;
	
	int height, width;
	App->window->GetScreenSize(width, height);

	/*glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(Scale), glm::vec3(0.0f, 1.0f, 0.0f));*/
	
	//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	//ourShader->setMat4("model", model);

	
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
	//App->componentsManager->DrawGameObject(*ourShader, this->root);
	App->componentsManager->DrawGameObject(*ourShader, this->root);
	//m_ModelLoader->Draw(*ourShader);
	//ourShader->use();
	/*glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/
	return UPDATE_CONTINUE;
}

