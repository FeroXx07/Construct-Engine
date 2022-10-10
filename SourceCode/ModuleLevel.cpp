#include "Globals.h"
#include "Application.h"
#include "ModuleLevel.h"
#include "Primitive.h"

#include <gl/glew.h>
#include "Vertex.h"

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
	srand(getpid());
	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(vec3(0, 0, 0));

	//ourShader = new Shader("SourceCode/Shaders/3.3.shader.vs", "SourceCode/Shaders/3.3.shader.fs"); // you can name your shader files however you like

	/*Vertices[0] = vec3(-1.0f, -1.0f, 0.0f);
	Vertices[1] = vec3(1.0f, -1.0f, 0.0f);
	Vertices[2] = vec3(0.0f, 1.0f, 0.0f);*/

	Vertex Vertices[4] = {};
	Vertices[0] = Vertex(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vertex(0.0f, -1.0f, 1.0f);
	Vertices[2] = Vertex(1.0f, -1.0f, 0.0f);
	Vertices[3] = Vertex(0.0f, 1.0f, 0.0f);

	unsigned int Indices[] = { 0, 3, 1,
						   1, 3, 2,
						   2, 3, 0,
						   0, 1, 2 };

	//// set up vertex data (and buffer(s)) and configure vertex attributes

	glGenVertexArrays(1, (GLuint*)&VAO);
	glGenBuffers(1, (GLuint*)&VBO);
	glGenBuffers(1, (GLuint*)&EBO);

	//// 1. bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	//// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	//// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// 4 now that we have the data in the buffer (vram), the gpu needs to know how to interpret that data
	//// 4.1 then set the vertex attributes pointers for the POSITION (remember 3 floats X,Y,Z)		
																					//STRIDE 0  1  2  3  4  5  6  7
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0); // stride (dist from x1 to x2) of 7 because -- X1-Y1-Z1-R1-G1-B1-A1-X2-Y2-Z2-R2-G2-B2-A1
	glEnableVertexAttribArray(0);
	//// 4.2 then set the vertex attributes pointers for the COLOR (remember 4 floats R,G,B,A)		
																							 //STRIDE 0  1  2  3  4  5  6  7
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))); // stride (dist from x1 to x2) of 7 because -- X1-Y1-Z1-R1-G1-B1-A1-X2-Y2-Z2-R2-G2-B2-A1
	glEnableVertexAttribArray(1);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	debug_draw = false;
	ourShader = new Shader("SourceCode/Shaders/3.3.shader.vert", "SourceCode/Shaders/3.3.shader.frag"); // you can name your shader files however you like

	//gScaleLocation = glGetUniformLocation(ourShader->ID, "gScale");
	//if (gScaleLocation == -1)
	//	LOG("Error getting uniform location of 'gScale'");
	//
	//gRotLocation = glGetUniformLocation(ourShader->ID, "gRotation");
	//if (gRotLocation == -1)
	//	LOG("Error getting uniform location of 'gRotation'");

	gWorldLocation = glGetUniformLocation(ourShader->ID, "gWorld");
	if (gWorldLocation == -1)
		LOG("Error getting uniform location of 'gWorld'");

	return ret;
}

// Load assets
bool ModuleLevel::CleanUp()
{
	LOG("Unloading Intro scene");

	if (ourShader != nullptr)
		delete ourShader;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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
	/*AngleInRadians += Delta;
	if ((AngleInRadians >= 1.5708f) || (AngleInRadians <= -1.5708f))
		Delta *= -1.0f;*/

	//matrix = math::float4x4(
	//	cosf(AngleInRadians), -sinf(AngleInRadians), 0.0f, Scale * 2,
	//	sinf(AngleInRadians), cosf(AngleInRadians), 0.0f, Scale,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f);

	matrix = float4x4::identity;
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &matrix[0][0]);
	return UPDATE_CONTINUE;
}

update_status ModuleLevel::PostUpdate(float dt)
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
	ourShader->use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	
	
	return UPDATE_CONTINUE;
}

