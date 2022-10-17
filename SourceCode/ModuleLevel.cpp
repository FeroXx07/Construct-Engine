#include "Globals.h"
#include "Application.h"
#include "ModuleLevel.h"

#include <gl/glew.h>
#include "Vertex.h"
#include "stb_image.h"
#include "glmath.h"

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

	Vertex Vertices[8];
	Vertices[0] = Vertex(0.5f, 0.5f, 0.5f);   // FRONT TOP RIGHT
	Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f); // REAR TOP LEFT
	Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f);  // FRONT TOP LEFT
	Vertices[3] = Vertex(0.5f, -0.5f, -0.5f); // REAR BOTTOM RIGHT
	Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f);// REAR BOTTOM LEFT 
	Vertices[5] = Vertex(0.5f, 0.5f, -0.5f);  // REAR TOP RIGHT
	Vertices[6] = Vertex(0.5f, -0.5f, 0.5f);  // FRONT BOTTOM RIGHT
	Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f); // FRONT BOTTOM LEFT

	unsigned int Indices[] = { 0, 2, 7,
							   1, 3, 4,
							    5, 6, 3,
							   7, 3, 6,
							   2, 4, 7,
							   0, 7, 6,
							   0, 5, 1,
							   1, 5, 3,
							   5, 0, 6,
							   7, 4, 3,
							   2, 1, 4,
							   0, 2, 7
	};


	ourShader = new Shader("Resources/Shaders/simple.vert", "Resources/Shaders/simple.frag"); // you can name your shader files however you like

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//// 4.2 then set the vertex attributes pointers for the COLOR (remember 4 floats R,G,B,A)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // stride (dist from x1 to x2) of 7 because -- X1-Y1-Z1-R1-G1-B1-A1-X2-Y2-Z2-R2-G2-B2-A1
	glEnableVertexAttribArray(1);

	//// 4.3 texture coord attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
 //   glEnableVertexAttribArray(1);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	debug_draw = true;
	
	matrix = float4x4::identity;
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
	glDeleteBuffers(1, &EBO);

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

	Scale += 0.2f;
	//Scale = 1.0f;
	//Pipeline p;
	//p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
	//p.WorldPos(sinf(Scale), 0.0f, 0.0f);
	//p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);
	//matrix = *p.GetTrans();
	//glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &matrix[0][0]);
	//glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());
	/*float4x4 rotation(
		cosf(Scale), 0.0f, -sinf(Scale), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinf(Scale), 0.0f, cosf(Scale), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	float4x4 translation(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 2.0f,
		0.0f, 0.0f, 0.0f, 1.0f);*/

	matrix = float4x4::identity;
	float4x4 rotation = matrix.RotateY(Scale);
	rotation.SetTranslatePart(0, 0, 2);
	
	int height, width;
	App->window->GetScreenSize(width, height);
	mat4x4 mModel;
	mModel.rotate(Scale, vec3(0.0f, 1.0f, 0.0f));
	mat4x4 mView;
	mView.translate(0.0f, 0.0f, 0.0f);
	mat4x4 mProj;
	mProj.perspective(DEGTORAD * 45.0f, (float)width / (float)height, 0.1f, 100.0f);
	//m.translate(0, 0, 2);

	float FOV = 90.0f;
	float tanHalfFOV = tanf(DegToRad((FOV / 2.0f)));
	float f = 1 / tanHalfFOV;
	//glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	
	
	float4x4 projection = float4x4(
		f, 0, 0, 0,
		0, f, 0, 0,
		0, 0, 1, 0, 
		0, 0, 1, 1);

	float4x4 finalMatrix = (rotation);
	std::cout << finalMatrix << std::endl;
	ourShader->setMat4("model", mModel);
	ourShader->setMat4("view", mView);
	ourShader->setMat4("projection", mProj);
	
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

	// bind textures on corresponding texture units
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture2);

	ourShader->use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	return UPDATE_CONTINUE;
}

