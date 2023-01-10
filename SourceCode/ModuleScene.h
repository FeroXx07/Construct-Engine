#pragma once
#include "Module.h"
#include "Globals.h"

#include "shader.h"
#include "ModelLoader.h"
#include "GameObject.h"

#include <list>
#include "Primitive.h"

enum StateEditor
{
	ON_EDITOR,
	ON_PAUSE,
	ON_PLAYING
};

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void Draw();
	StateEditor editorState = ON_EDITOR;
	void ChangeEditorState(StateEditor newState);
	GameObject* CreateGameObject(string const& path, string name);
	GameObject* CreateEmptyGameObject(string name);
	GameObject* CreateCamera(string name, GameObject* destinationGO = nullptr);
	
	GameObject* root = nullptr;

	bool debug_draw = true;
	Shader* modelsShader = nullptr;
	Shader* screenShader = nullptr;

	float Scale = 0.0f;
	ModelLoader* m_ModelLoader = nullptr;

	glm::mat4 projection;

	void SaveSceneJson();
	void LoadSceneJson();
	void LoadSceneJson(const char* sceneName);
	void DeleteScene();
	void To_Json(json& j, const GameObject* go);
	void LoadConstraints(json& j, const GameObject* goParent);
	GameObject* From_Json(const json& j, const GameObject* goParent);

};

