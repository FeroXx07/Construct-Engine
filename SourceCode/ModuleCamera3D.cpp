#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	/*CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 0.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);*/
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	editorCamera = new ComponentCamera("editorCamera", glm::vec3(0.0f, 0.0f, 10.0f));
	cameras.push_back(editorCamera);
	editorCamera->m_Framebuffer = 0; // Default buffer!

	int height, width;
	App->window->GetScreenSize(width, height);
	/*lastX = width / 2.0f;
	lastY = height / 2.0f;*/

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	for (std::vector<ComponentCamera*>::iterator i = cameras.begin(); i != cameras.end(); ++i)
	{
		delete* i;
		*i = nullptr;
	}
		
	cameras.clear();

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	//vec3 newPos(0,0,0);
	for (auto el : cameras)
	{
		el->Update();
	}
	float speed = 3.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	//if(App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT) newPos.z += 10*speed;
	//if(App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) newPos.z -= 10*speed;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) editorCamera->m_Camera->ProcessKeyboard(Camera_Movement::FORWARD, speed);
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) editorCamera->m_Camera->ProcessKeyboard(Camera_Movement::BACKWARD, speed);
	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)  editorCamera->m_Camera->ProcessKeyboard(Camera_Movement::LEFT, speed);
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) editorCamera->m_Camera->ProcessKeyboard(Camera_Movement::RIGHT, speed);

	//Position += newPos;
	//Reference += newPos;

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int xposIn = App->input->GetMouseXMotion();
		int yposIn = -App->input->GetMouseYMotion();
		
		editorCamera->m_Camera->ProcessMouseMovement(xposIn, yposIn);
	}

	int zScrollIn = App->input->GetMouseZ();
	//LOG("Mouse scroll is %d", zScrollIn);
	if (zScrollIn != 0)
	{
		editorCamera->m_Camera->ProcessMouseScroll((float)zScrollIn);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_UP)
	{
		if (App->uiManager->m_CurrentSelectedNode != nullptr)
			editorCamera->m_Camera->LookAt(App->uiManager->m_CurrentSelectedNode->GetTransform()->GetTranslate());
	}

	return UPDATE_CONTINUE;
}
