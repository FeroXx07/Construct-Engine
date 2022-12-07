#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleComponentSys.h"
#include "ModulePhysics3D.h"
#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "ComponentMesh.h"
#include "ComponentCamera.h"
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

	modelsShader = nullptr;
	screenShader = nullptr;
	m_ModelLoader = nullptr;

	modelsShader = new Shader("Resources/Shaders/model_loading.vert", "Resources/Shaders/model_loading.frag"); // you can name your shader files however you like
	screenShader = new Shader("Resources/Shaders/framebuffer_screen.vert", "Resources/Shaders/framebuffer_screen.frag"); // you can name your shader files however you like
	screenShader->use();
	screenShader->setInt("screenTexture", 0);

	srand(_getpid());

	root = new GameObject("RootNode");
	ComponentTransform* rootTrans = new ComponentTransform();
	root->AssignComponent(rootTrans);
	GameObject* mainCamera = CreateCamera("Main Camera");
	glm::vec3 posMainCamera = mainCamera->GetTransform()->GetTranslate();
	posMainCamera.x += -5;
	mainCamera->GetTransform()->SetTranslate(posMainCamera);
	App->camera->editorCamera->GenerateBuffers(App->scene->screenShader);
	//mainCamera->GetCamera()->camera->Yaw += 180.0f;// rotate the camera's yaw 180 degrees around
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(false);

	m_ModelLoader = new ModelLoader(App->componentsManager, App->physics3D);

	//CreateGameObject("Assets/BakerHouse.fbx", "BakerHouse");
	CreateGameObject("Assets/street/street2.fbx", "StreetScene");
	GameObject* ground = CreateEmptyGameObject("Ground");
	ground->GetTransform()->SetScale(glm::vec3(100, 100, 100));
	ground->SetPhysBody(App->physics3D->AddBodyCube(ground->GetTransform()->GetLocal(), 1.0f));
	debug_draw = false;
	SaveSceneJson();
	return ret;
}

update_status ModuleScene::PreUpdate(float dt)
{
	App->componentsManager->UpdateAllTransforms(this->root);
	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	if (modelsShader != nullptr)
		delete modelsShader;

	if (screenShader != nullptr)
		delete screenShader;

	if (m_ModelLoader != nullptr)
		delete m_ModelLoader;
	
	return true;
}

void ModuleScene::Draw()
{
	if (debug_draw == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	for (auto camera : App->camera->cameras)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, camera->m_Framebuffer); // If fbo=0 then default frame buffer, as the case for editorCamera.
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		// make sure we clear the framebuffer's content
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// The verticalFov (is zoom), the horizontalFov needs to be adjusted as FovY changes
		int height, width;
		App->window->GetScreenSize(width, height);
		float aspectRatio = (float)width / (float)height;
		float nearPlane = 1.0f;
		float farPlane = 50.0f;
		string compareString = "editorCamera";
		if (camera->m_Name == compareString)
		{
			nearPlane = 0.1f;
			farPlane = 1000.0f;
		}
			

		camera->m_Camera->projection = glm::perspective(glm::radians(camera->m_Camera->Zoom), aspectRatio, nearPlane, farPlane);
		modelsShader->setMat4("projection", camera->m_Camera->projection);

		modelsShader->use();
		glm::mat4 view = camera->m_Camera->GetViewMatrix();
		modelsShader->setMat4("view", view);
		camera->m_Camera->frustum = MyFrustum(camera->m_Camera->projection * camera->m_Camera->GetViewMatrix());

		App->componentsManager->DrawGameObject(camera, *modelsShader, this->root, this->root->GetTransform()->GetLocal());


		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(camera->m_Camera->projection));
		glMatrixMode(GL_MODELVIEW);
		glm::mat4 MV = view;
		glLoadMatrixf(glm::value_ptr(MV));
		
		//App->physics3D->world->debugDrawWorld();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // If fbo=0 then default frame buffer, as the case for editorCamera.
	// make sure we clear the framebuffer's content
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glm::mat4 view = App->camera->editorCamera->m_Camera->GetViewMatrix();
	modelsShader->setMat4("view", view);
	App->componentsManager->DrawGameObject(App->camera->editorCamera, *modelsShader, this->root, this->root->GetTransform()->GetLocal());
	App->physics3D->world->debugDrawWorld();*/
	for (auto camera : App->camera->cameras)
	{
		camera->RenderWindow();
	}

	modelsShader->use();
}

void ModuleScene::ChangeEditorState(StateEditor newState)
{
	switch (newState)
	{
	case ON_EDITOR:
	{
		if (editorState == StateEditor::ON_PLAYING || editorState == StateEditor::ON_PAUSE)
		{
			App->physics3D->delta = 0.0000001f;
			DeleteScene();
			LoadSceneJson();
			editorState = ON_EDITOR;
		}
		break;
	}
		
	case ON_PAUSE:
	{
		if (editorState == StateEditor::ON_PLAYING)
		{
			editorState = ON_PAUSE;
		}
		break;
	}
	
	case ON_PLAYING:
	{
		if (editorState == StateEditor::ON_EDITOR)
		{
			SaveSceneJson();
			editorState = ON_PLAYING;
		}
		else if (editorState == StateEditor::ON_PAUSE)
		{
			editorState = ON_PLAYING;
		}
		break;
	}
		
	default:
		break;
	}
}

GameObject* ModuleScene::CreateGameObject(string const& path, string name)
{
	//name = name.substr(name.find_last_of("/\\") + 1);
	//GameObject* go = new GameObject(name);
	//go->SetParentAndChild(this->root);
	//ComponentTransform* trans = new ComponentTransform();
	//go->AssignComponent(trans);
	//GameObject* go = CreateEmptyGameObject(name);
	m_ModelLoader->LoadModelFrom_aiScene(path, this->root);
	return nullptr;
}

GameObject* ModuleScene::CreateEmptyGameObject(string name)
{
	// Create new GameObject
	GameObject* go = new GameObject(name);
	go->SetParent(this->root);
	this->root->SetChild(go);
	// Create a ComponentTransform and assign it
	ComponentTransform* transform = new ComponentTransform();
	transform->SetLocalMatrix(glm::mat4(1.0f));
	transform->SetWorldMatrix(this->root->GetTransform()->GetWorld()*glm::mat4(1.0f));
	go->AssignComponent(transform);
	return go;
}

GameObject* ModuleScene::CreateCamera(string name, GameObject* destinationGO)
{
	if (destinationGO != nullptr)
	{
		string tmp = "Camera_";
		tmp += destinationGO->m_Name;
		ComponentCamera* cameraComp = new ComponentCamera(tmp.c_str());
		cameraComp->GenerateBuffers(screenShader);
		App->camera->cameras.push_back(cameraComp);
		destinationGO->AssignComponent(cameraComp);
		return destinationGO;
	}
	else
	{
		GameObject* cameraGO = CreateEmptyGameObject(name);
		int totalCameras = App->camera->cameras.size();
		string tmp = "Camera_";
		tmp += std::to_string(totalCameras);
		ComponentCamera* cameraComp = new ComponentCamera(tmp.c_str());
		cameraComp->GenerateBuffers(screenShader);
		App->camera->cameras.push_back(cameraComp);
		cameraGO->AssignComponent(cameraComp);
		return cameraGO;
	}
}

void ModuleScene::SaveSceneJson()
{
	// Create null value
	json jSonGameObjects;

	To_Json(jSonGameObjects, root);

	// default serialization 
	auto s1 = jSonGameObjects.dump();
	//std::cout << jSettings << std::endl;

	std::ofstream file("Resources/Scenes/scene.json");
	if (file.is_open())
	{
		file << jSonGameObjects;
		file.close();
	}
}

void ModuleScene::LoadSceneJson()
{
	std::ifstream file("Resources/Scenes/scene.json");
	if (file.is_open())
	{
		json data = json::parse(file);
		for (auto el : data["Game Objects"])
		{
			From_Json(el, root);
		}
		file.close();
	}
	// Load all children from root node
	App->physics3D->Start();
}

void ModuleScene::DeleteScene()
{
	App->physics3D->CleanUp();
	App->physics3D->Start();
	App->uiManager->SetNullSelected();
	int size = root->m_Children.size();
	for (int i = size-1; i >= 0; i--)
	{
		// Remove camera from camera list
		if (root->m_Children[i]->m_HasComponentCamera)
		{
			int totalCams = App->camera->cameras.size();
			for (int j = 0; j < totalCams; j++)
			{
				if (root->m_Children[i]->GetCamera() == App->camera->cameras[j])
					App->camera->cameras.erase(App->camera->cameras.begin()+j);
			}
		}

		delete root->m_Children[i];
	}

}

void ModuleScene::To_Json(json& json_, const GameObject* go_)
{
	// add via member function
	string id = std::to_string(go_->id);
	// Id
	json_["Game Objects"][id]["UID"] = go_->id;
	// Parent id
	if (go_->GetParentConst() != nullptr)
		json_["Game Objects"][id]["ParentUID"] = go_->GetParentConst()->id;
	// Name
	json_["Game Objects"][id] += {"Name", go_->m_Name.c_str()};
	//Transform
	ComponentTransform* t = go_->GetTransformConst();
	json_["Game Objects"][id]["Translation"] = {t->GetTranslate().x, t->GetTranslate().y, t->GetTranslate().z};
	json_["Game Objects"][id]["Scale"] = {t->GetScaling().x, t->GetScaling().y, t->GetScaling().z};
	json_["Game Objects"][id]["Rotation"] = {t->GetRotationQuat().w, t->GetRotationQuat().x, t->GetRotationQuat().y, t->GetRotationQuat().z};
	
	//Mesh
	if (go_->m_HasComponentMesh)
	{
		json_["Game Objects"][id]["Mesh"]["Exists"] = true;
		json_["Game Objects"][id]["Mesh"]["Name"] = go_->GetMeshConst()->GetMesh()->name;
	}
	else
		json_["Game Objects"][id]["Mesh"]["Exists"] = false;

	// Material
	if (go_->m_HasComponentMaterial)
	{
		json_["Game Objects"][id]["Material"]["Exists"] = true;
		ComponentMaterial* m = go_->GetMaterialConst();
		int total = m->m_Textures.size();
		json_["Game Objects"][id]["Material"]["Name"] = m->m_MaterialName;
		json_["Game Objects"][id]["Material"]["Totaltextures"] = total;
		for (int i = 0; i < total; i++)
		{
			string tex = "texture";
			tex += std::to_string(i);
			json_["Game Objects"][id]["Material"][tex] = m->m_Textures.at(i).path;
		}
	}
	else
		json_["Game Objects"][id]["Material"]["Exists"] = false;

	// Camera
	if (go_->m_HasComponentCamera)
	{
		json_["Game Objects"][id]["Camera"]["Exists"] = true;
		json_["Game Objects"][id]["Camera"]["Position"] = { t->GetTranslate().x, t->GetTranslate().y, t->GetTranslate().z };
		json_["Game Objects"][id]["Camera"]["Name"] = go_->GetCameraConst()->m_Name;
	}
	else
		json_["Game Objects"][id]["Camera"]["Exists"] = false;


	for (auto children : go_->m_Children)
	{
		To_Json(json_, children);
	}
}

GameObject* ModuleScene::From_Json(const json& j, const GameObject* goParent)
{
	string name = j["Name"];
	string rootnode = "RootNode";

	if (name != rootnode)
	{
		// New gameobject
		GameObject* newGo = new GameObject(name);
		newGo->id = j["UID"];
		uint parentId = j["ParentUID"];
		newGo->SetParent(root->FindById(parentId));
		newGo->GetParent()->SetChild(newGo);
		// New transform
		glm::vec3 Translation, Scaling;
		glm::quat Rotation;
		std::vector<float>l = j["Translation"];
		Translation = vec3(l.at(0), l.at(1), l.at(2));
		std::vector<float>l2 = j["Scale"];
		Scaling = vec3(l2.at(0), l2.at(1), l2.at(2));
		std::vector<float>l3 = j["Rotation"];
		Rotation = quat(l3.at(0), l3.at(1), l3.at(2), l3.at(3));
		ComponentTransform* newTransform = new ComponentTransform(Translation, Scaling, Rotation);
		newGo->AssignComponent(newTransform);

		// New mesh
		if (j["Mesh"]["Exists"])
		{
			Mesh* mesh = m_ModelLoader->LoadFromCustomFormat(name.c_str());
			ComponentMesh* newMeshComp = new ComponentMesh(mesh);
			newGo->AssignComponent(newMeshComp);
			newGo->GenerateBoundingBoxes();
		}

		if (j["Material"]["Exists"])
		{
			ComponentMaterial* newMatComp = new ComponentMaterial();
			newGo->AssignComponent(newMatComp);
			//m_ModelLoader->LoadTextureIntoGameObject()
			std::vector<Texture> tex;
			/*newMatComp->PassTextures();*/
			int totalTex = j["Material"]["Totaltextures"];
			for (int i = 0; i < totalTex; i++)
			{
				string tex = "texture";
				tex += std::to_string(i);
				tex = j["Material"][tex];
				m_ModelLoader->LoadTextureIntoGameObject(tex.c_str(), newGo);
			}
			newMatComp->m_MaterialName = j["Material"]["Name"];
			
		}

		if (j["Camera"]["Exists"])
		{
			string camName = j["Camera"]["Name"];
			std::vector<float>pos = j["Camera"]["Position"];
			vec3 cameraPos = vec3(pos.at(0), pos.at(1), pos.at(2));
			CreateCamera(camName, newGo);
			newGo->GetCamera()->m_Camera->Position = cameraPos;
		}
		/*json_["Game Objects"][id]["Camera"]["Position"] = { t->GetTranslate().x, t->GetTranslate().y, t->GetTranslate().z };
		json_["Game Objects"][id]["Camera"]["Name"] = go_->GetCameraConst()->m_Name;*/

		return newGo;
	}
	
	return root;
}


// Update
update_status ModuleScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		LoadSceneJson();
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_UP)
		SaveSceneJson();
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_UP)
		DeleteScene();
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	Draw();
	return UPDATE_CONTINUE;
}

