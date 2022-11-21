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
GLuint LoadTextureFromFile(const char* path, const string& directory, uint& height, uint& width, uint &nComponents, bool gamma = false);
class ModelLoader
{
public:

    Texture* checkerTexture = nullptr;
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
    void LoadTextureIntoGameObject(string const& path, GameObject* go);
private:
  
    // processes a aiNode in a recursive fashion. Processes each individual aiMesh located at the aiNode and repeats this process on its children nodes (if any).
    void ProcessNode(aiNode* aiNode, const aiScene* aiScene, GameObject* parent, ComponentTransform parentWorld);
    Mesh* CreateMesh(aiMesh* aiMesh, const aiScene* scene);
    
    vector<Texture> LoadMaterialTextures(aiMaterial* aiMaterial, aiTextureType aiTextureType, string typeName);

    void GetTransformationFromNode(aiNode* aiNode, ComponentTransform* trans);
    bool IsDummyNode(const aiNode& assimpNode);

public:
    // Save And Load mesh from custom format
    void Save(const Mesh* mesh, const char* filename);
    Mesh* LoadFromCustomFormat(const char* filename, Mesh* compareMesh = nullptr);
};

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
    glm::mat4 to;


    to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
    to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
    to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
    to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

    return to;
}
#endif
