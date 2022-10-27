#ifndef _MODEL_H
#define _MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <physfs/physfs.h>

class GameObject;
class ModuleComponentSys;
class ComponentTransform;

using namespace std;
GLuint LoadTextureFromFile(const char* path, const string& directory, bool gamma = false);
class ModelLoader
{
public:

    vector<Texture> textures_loaded;	
    //vector<Mesh>    meshesList;
    string directory;
    bool gammaCorrection;
    uint meshesSize = 0;

    ModuleComponentSys* componentSystem = nullptr;
    ModelLoader(ModuleComponentSys* sys, bool gamma = false);
    ~ModelLoader();
 
    // draws the model, and all its meshesList
    void Draw(Shader& shader);

    // loads from aiScene
    void LoadModelFrom_aiScene(string const& path, GameObject* parent);
private:
  
    // processes a aiNode in a recursive fashion. Processes each individual aiMesh located at the aiNode and repeats this process on its children nodes (if any).
    void ProcessNode(aiNode* aiNode, const aiScene* aiScene, GameObject* parent);
    Mesh* CreateMesh(aiMesh* aiMesh, const aiScene* scene);
    
    vector<Texture> LoadMaterialTextures(aiMaterial* aiMaterial, aiTextureType aiTextureType, string typeName);

    void GetTransformationFromNode(aiNode* aiNode, ComponentTransform* trans);
    bool IsDummyNode(const aiNode& assimpNode);
};
#endif
