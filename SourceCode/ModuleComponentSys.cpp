#include "Application.h"
#include "ModuleComponentSys.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"

ModuleComponentSys::ModuleComponentSys(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//m_MeshSystem = nullptr;
}

ModuleComponentSys::~ModuleComponentSys()
{
}

bool ModuleComponentSys::Start()
{
	//m_MeshSystem = new MeshSystem();
	return true;
}

update_status ModuleComponentSys::Update(float dt)
{
	return UPDATE_CONTINUE;
}


void ModuleComponentSys::DrawGameObject(ComponentCamera* camera, Shader& shader, GameObject* node, glm::mat4x4 parentWorld)
{
	shader.use();
	bool isRoot = false;
	if (node->GetParent() == nullptr)
		isRoot = true;
	glm::mat4x4 world = parentWorld;
	MyFrustum* frustum = &camera->m_Camera->frustum;
	if (!isRoot)
	{
		ComponentTransform* local = node->GetTransform();
		local->Update();
		world = local->Combine(parentWorld);
		local->SetWorldMatrix(world);

		if (node->m_HasComponentMesh)
		{
			ComponentMesh* mesh = node->GetMesh();
			shader.setMat4("model", world);
			if (node->m_HasComponentMaterial)
			{
				ComponentMaterial* material = node->GetMaterial();
				AABB* box = &node->m_Aabb;
				glm::vec3 minP = glm::vec3(box->minPoint.x, box->minPoint.y, box->minPoint.z);
				glm::vec3 maxP = glm::vec3(box->maxPoint.x, box->maxPoint.y, box->maxPoint.z);
				//bool isVisible = frustum->IsBoxVisible(minP, maxP);

				//if (isVisible)
				mesh->GetMesh()->RenderMesh(shader, material);
				//else
					//std::cout << node->m_Name << " is not visible !" << std::endl << std::endl;
			}

			DrawNormals(camera, mesh, local);
			DrawBoundingBoxes(camera, local, node);
		}

		if (node->m_HasComponentCamera)
		{
			DrawFrustum(camera, node->GetCameraConst(),local);
		}
	}
	for (auto c : node->m_Children)
	{
		DrawGameObject(camera, shader, c, world);
	}
	
}

void ModuleComponentSys::AddTextureToGameObject(string const& path)
{
	if (App->uiManager->m_CurrentSelectedNode != nullptr)
		App->scene->m_ModelLoader->LoadTextureIntoGameObject(path, App->uiManager->m_CurrentSelectedNode);
}


void ModuleComponentSys::DrawNormals(ComponentCamera* camera, ComponentMesh* mesh, ComponentTransform* transform)
{
	if (mesh->m_DisplayNormals)
	{
		glm::mat4 view = camera->m_Camera->GetViewMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf((const GLfloat*)&App->scene->projection[0]);
		glMatrixMode(GL_MODELVIEW);
		glm::mat4 MV = view * transform->GetWorld();
		glLoadMatrixf((const GLfloat*)&MV[0]);
		glUseProgram(0);
		glColor3f(0, 0, 1);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		for (auto index : mesh->GetMesh()->indices)
		{
			glm::vec3 p = mesh->GetMesh()->vertices[index].m_Position;
			glVertex3f(p.x, p.y, p.z);
			glm::vec3 o = glm::normalize(mesh->GetMesh()->vertices[index].m_Normal);
			p += o * 0.1f;
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
		glLineWidth(1.0f);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleComponentSys::DrawBoundingBoxes(ComponentCamera* camera, ComponentTransform* transform, GameObject* go)
{
	glm::mat4 view = camera->m_Camera->GetViewMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*)&camera->m_Camera->projection[0]);
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 MV = view * transform->GetWorld();
	glLoadMatrixf((const GLfloat*)&MV[0]);
	glUseProgram(0);

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	math::float3 c[8];
	AABB* b = &go->m_Aabb;
	math::float3 p0 = b->CornerPoint(0);
	math::float3 p1 = b->CornerPoint(1);
	math::float3 p2 = b->CornerPoint(2);
	math::float3 p3 = b->CornerPoint(3);
	math::float3 p4 = b->CornerPoint(4);
	math::float3 p5 = b->CornerPoint(5);
	math::float3 p6 = b->CornerPoint(6);
	math::float3 p7 = b->CornerPoint(7);
	
	glColor3f(0, 255, 0);

	// Bottom face
	glVertex3f(p0.x,p0.y,p0.z); glVertex3f(p4.x,p4.y,p4.z);
	glVertex3f(p4.x, p4.y, p4.z); glVertex3f(p6.x,p6.y,p6.z);
	glVertex3f(p6.x, p6.y, p6.z); glVertex3f(p2.x,p2.y,p2.z);
	glVertex3f(p2.x, p2.y, p2.z); glVertex3f(p0.x, p0.y, p0.z);

	// Left face
	glVertex3f(p0.x, p0.y, p0.z); glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p2.x, p2.y, p2.z); glVertex3f(p3.x, p3.y, p3.z);
	glVertex3f(p3.x, p3.y, p3.z); glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p1.x, p1.y, p1.z); glVertex3f(p0.x, p0.y, p0.z);

	// Top face
	glVertex3f(p1.x, p1.y, p1.z); glVertex3f(p5.x, p5.y, p5.z);
	glVertex3f(p5.x, p5.y, p5.z); glVertex3f(p7.x, p7.y, p7.z);
	glVertex3f(p7.x, p7.y, p7.z); glVertex3f(p3.x, p3.y, p3.z);
	glVertex3f(p3.x, p3.y, p3.z); glVertex3f(p1.x, p1.y, p1.z);

	// Right face
	glVertex3f(p5.x, p5.y, p5.z); glVertex3f(p4.x, p4.y, p4.z);
	glVertex3f(p4.x, p4.y, p4.z); glVertex3f(p6.x, p6.y, p6.z);
	glVertex3f(p6.x, p6.y, p6.z); glVertex3f(p7.x, p7.y, p7.z);
	glVertex3f(p7.x, p7.y, p7.z); glVertex3f(p5.x, p5.y, p5.z);

	glEnd();
	glLineWidth(1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleComponentSys::DrawFrustum(ComponentCamera* camera, ComponentCamera* frustumCamera, ComponentTransform* transform)
{
	
	glm::mat4 view = camera->m_Camera->GetViewMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(camera->m_Camera->projection));
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 local(1.0f);

	glm::mat4 MV = view* transform->GetLocal();
	glLoadMatrixf(glm::value_ptr(MV));
	glUseProgram(0);

	glLineWidth(3.0f);
	glBegin(GL_LINES);
	math::float3 c[8];
	MyFrustum* f = &frustumCamera->m_Camera->frustum;
	glColor3f(255, 0, 0);

	// Near face
	glVertex3f(f->P(0).x, f->P(0).y, f->P(0).z); glVertex3f(f->P(1).x, f->P(1).y, f->P(1).z);
	glVertex3f(f->P(1).x, f->P(1).y, f->P(1).z); glVertex3f(f->P(3).x, f->P(3).y, f->P(3).z);
	glVertex3f(f->P(3).x, f->P(3).y, f->P(3).z); glVertex3f(f->P(2).x, f->P(2).y, f->P(2).z);
	glVertex3f(f->P(2).x, f->P(2).y, f->P(2).z); glVertex3f(f->P(0).x, f->P(0).y, f->P(0).z);

	// Front face
	glVertex3f(f->P(4).x, f->P(4).y, f->P(4).z); glVertex3f(f->P(5).x, f->P(5).y, f->P(5).z);
	glVertex3f(f->P(5).x, f->P(5).y, f->P(5).z); glVertex3f(f->P(7).x, f->P(7).y, f->P(7).z);
	glVertex3f(f->P(7).x, f->P(7).y, f->P(7).z); glVertex3f(f->P(6).x, f->P(6).y, f->P(6).z);
	glVertex3f(f->P(6).x, f->P(6).y, f->P(6).z); glVertex3f(f->P(4).x, f->P(4).y, f->P(4).z);

	// Left face
	glVertex3f(f->P(0).x, f->P(0).y, f->P(0).z); glVertex3f(f->P(1).x, f->P(1).y, f->P(1).z);
	glVertex3f(f->P(1).x, f->P(1).y, f->P(1).z); glVertex3f(f->P(5).x, f->P(5).y, f->P(5).z);
	glVertex3f(f->P(5).x, f->P(5).y, f->P(5).z); glVertex3f(f->P(4).x, f->P(4).y, f->P(4).z);
	glVertex3f(f->P(4).x, f->P(4).y, f->P(4).z); glVertex3f(f->P(0).x, f->P(0).y, f->P(0).z);

	// Right face
	glVertex3f(f->P(2).x, f->P(2).y, f->P(2).z); glVertex3f(f->P(3).x, f->P(3).y, f->P(3).z);
	glVertex3f(f->P(3).x, f->P(3).y, f->P(3).z); glVertex3f(f->P(7).x, f->P(7).y, f->P(7).z);
	glVertex3f(f->P(7).x, f->P(7).y, f->P(7).z); glVertex3f(f->P(6).x, f->P(6).y, f->P(6).z);
	glVertex3f(f->P(6).x, f->P(6).y, f->P(6).z); glVertex3f(f->P(2).x, f->P(2).y, f->P(2).z);

	glEnd();
	glLineWidth(1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleComponentSys::UpdateAllTransforms(GameObject* rootNode)
{
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//rootNode->UpdateBody();

	//for (auto c : rootNode->m_Children)
	//{
	//	UpdateAllTransforms(c);
	//}
}



bool ModuleComponentSys::CleanUp()
{
	return true;
}

