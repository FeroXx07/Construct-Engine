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
using namespace std;
GLuint LoadTextureFromFile(const char* path, const string& directory, bool gamma = false);
class Model
{
public:

    vector<Texture> textures_loaded;	
    vector<Mesh>    meshesList;
    string directory;
    bool gammaCorrection;
    uint meshesSize = 0;

    Model(string const& path, bool gamma = false);
    ~Model();

    // draws the model, and all its meshesList
    void Draw(Shader& shader);

private:
    // loads from aiScene
    void LoadModelFrom_aiScene(string const& path);

    // processes a aiNode in a recursive fashion. Processes each individual aiMesh located at the aiNode and repeats this process on its children nodes (if any).
    void ProcessNode(aiNode* aiNode, const aiScene* aiScene);
    Mesh CreateMesh(aiMesh* aiMesh, const aiScene* scene);
    
    vector<Texture> LoadMaterialTextures(aiMaterial* aiMaterial, aiTextureType aiTextureType, string typeName);
};
#endif
