#ifndef __ModuleModels_H__
#define __ModuleModels_H__
#include "Module.h"
#include "Globals.h"
#include "Model.h"

class ModuleModels : public Module
{
public:
	ModuleModels(Application* app, bool start_enabled = true);
	~ModuleModels();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	vector<Model> modelsList;
};
#endif