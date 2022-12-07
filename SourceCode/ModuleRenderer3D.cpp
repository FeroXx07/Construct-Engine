#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include <gl/glew.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include "glut.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "glew32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "glew32s.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled), context(NULL)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(App->window->GetVsync() && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.1f,0.6f, 0.4f, 1.0f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glewVersion = LOGr("Using Glew %s", glewGetString(GLEW_VERSION));
		glewVersion = glewVersion.substr(glewVersion.find_last_of(":") + 1);
		// Should be 2.0
		vendor = LOGr("Vendor: %s", glGetString(GL_VENDOR));
		vendor = vendor.substr(vendor.find_last_of(":") + 1);

		rendInfo = LOGr("Renderer: %s", glGetString(GL_RENDERER));
		rendInfo = rendInfo.substr(rendInfo.find_last_of(":") + 1);

		supportedOpenGLversion = LOGr("OpenGL version supported %s", glGetString(GL_VERSION));
		supportedOpenGLversion = supportedOpenGLversion.substr(supportedOpenGLversion.find_last_of(":") + 1);

		GLSL = LOGr("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		GLSL = GLSL.substr(GLSL.find_last_of(":") + 1);

		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CW);
		//glCullFace(GL_BACK);
		
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		// Init Glew
		error = glewInit();
		//Check for error
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}
	
		std::string strVersion = (const char*)glGetString(GL_VERSION);
		strVersion = strVersion.substr(0, strVersion.find(" "));
		float number = std::atof(strVersion.c_str());

	}
	
	// Projection matrix for
	int height, width;
	App->window->GetScreenSize(width, height);
	OnResize(width, height);

	//InitImGui();
	//App->uiManager->EnableAndInit();
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());*/

	// light 0 on cam pos
	lights[0].SetPos(App->camera->editorCamera->m_Camera->Position.x, App->camera->editorCamera->m_Camera->Position.y, App->camera->editorCamera->m_Camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();



	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float nearPlane = 0.1f;
	float farPlane = 100.0f * 1.5f;
	if (App->camera->editorCamera != nullptr)
	{
		glm::mat4  projection = glm::perspective(glm::radians(App->camera->editorCamera->m_Camera->Zoom), (float)width / (float)height, nearPlane, farPlane);
		glLoadMatrixf(glm::value_ptr(projection));

	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

