#include "Globals.h"
#include "Application.h"
#include "ModuleLevel.h"
#include "Primitive.h"

#include <gl/glew.h>


ModuleLevel::ModuleLevel(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleLevel::~ModuleLevel()
{}

// Load assets
bool ModuleLevel::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	ourShader = nullptr;
	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(vec3(0, 0, 0));

	//ourShader = new Shader("SourceCode/Shaders/3.3.shader.vs", "SourceCode/Shaders/3.3.shader.fs"); // you can name your shader files however you like

	//float d = 200.0f;

	//for (float i = -d; i <= d; i += 1.0f)
	//{
	//	glVertex3f(i, 0.0f, -d);
	//	glVertex3f(i, 0.0f, d);
	//	glVertex3f(-d, 0.0f, i);
	//	glVertex3f(d, 0.0f, i);
	//}

	//// set up vertex data (and buffer(s)) and configure vertex attributes
	//// ------------------------------------------------------------------
	//
	//glGenVertexArrays(1, (GLuint*)&VAO);
	//glGenBuffers(1, (GLuint*)&VBO);
	//glGenBuffers(1, (GLuint*)&EBO);
	//// 1. bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//glBindVertexArray(VAO);

	//// 2. copy our vertices array in a vertex buffer for OpenGL to use
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//// 3. copy our index array in a element buffer for OpenGL to use
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//// 4. then set the vertex attributes pointers
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	//// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);


	//// uncomment this call to draw in wireframe polygons.
	////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Vertices[0] = vec3(-1.0f, -1.0f, 0.0f);
	Vertices[1] = vec3(1.0f, -1.0f, 0.0f);
	Vertices[2] = vec3(0.0f, 1.0f, 0.0f);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	ourShader = new Shader("SourceCode/Shaders/3.3.shader.vs", "SourceCode/Shaders/3.3.shader.fs"); // you can name your shader files however you like

	gScaleLocation = glGetUniformLocation(ourShader->ID, "gScale");
	if (gScaleLocation == -1)
		LOG("Error getting uniform location of 'gScale'");
	

	gTransLocation = glGetUniformLocation(ourShader->ID, "gTranslation");
	if (gScaleLocation == -1)
		LOG("Error getting uniform location of 'gTranslation'");

	

	return ret;
}

// Load assets
bool ModuleLevel::CleanUp()
{
	LOG("Unloading Intro scene");

	if (ourShader != nullptr)
		delete ourShader;

	return true;
}

void ModuleLevel::BeginDebugDraw()
{
}

void ModuleLevel::DebugDraw()
{
}

void ModuleLevel::EndDebugDraw()
{
}

// Update
update_status ModuleLevel::Update(float dt)
{
	Scale += Delta;
	if ((Scale >= 1.0f) || (Scale <= -1.0f))
		Delta *= -1.0f;

	translation = math::float4x4(
		1.0f, 0.0f, 0.0f, Scale * 2,
		0.0f, 1.0f, 0.0f, Scale,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(gTransLocation, 1, GL_TRUE, &translation[0][0]);
	glUniform1f(gScaleLocation, 1);
	return UPDATE_CONTINUE;
}

update_status ModuleLevel::PostUpdate(float dt)
{
	/*myPrimitive::Plane p(0, 1, 0, 0);
	p.axis = true;
	p.InnerRender();
	p.color.Set(255, 255, 255, 0);*/
	if (debug_draw == true)
	{
		BeginDebugDraw();
		DebugDraw();
		EndDebugDraw();
	}

	// render
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	ourShader->use();
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	// draw our first triangle
	//ourShader->use();
	//glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	////glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// glBindVertexArray(0); // no need to unbind it every time 

	return UPDATE_CONTINUE;
}

