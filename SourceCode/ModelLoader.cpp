#include "ModelLoader.h"
#include "Application.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "ModuleComponentSys.h"
ModelLoader::ModelLoader(ModuleComponentSys* sys, bool gamma) : gammaCorrection(gamma)
{
    componentSystem = sys;
    meshesSize = 0;
}

ModelLoader::~ModelLoader()
{
    componentSystem = nullptr;
}

void ModelLoader::Draw(Shader& shader)
{
 /*   for (unsigned int i = 0; i < meshesList.size(); i++)
        meshesList[i].Draw(shader);*/
}

void ModelLoader::LoadModelFrom_aiScene(string const& path, GameObject* parent)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    //const aiScene* scene = importer.ReadFileFromMemory(buffer, sizeof(buffer), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        // cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        LOG("ERROR::ASSIMP:: ");
        LOG( importer.GetErrorString());
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root aiNode recursively
    ProcessNode(scene->mRootNode, scene, parent);
}

void ModelLoader::ProcessNode(aiNode* aiNode, const aiScene* aiScene, GameObject* parent)
{
    GameObject* child = nullptr;
    Mesh* myMesh = nullptr;
    // process each aiMesh located at the current aiNode
    for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
    {
        // the aiNode object only contains indices to index the actual objects in the aiScene. 
        // the aiScene contains all the data, aiNode is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = aiScene->mMeshes[aiNode->mMeshes[i]];

        // Create new GameObject
        child = new GameObject(aiNode->mName.C_Str());
        child->SetParentAndChild(parent);

        // Create a ComponentMesh and fill it
        ComponentMesh* compMesh = new ComponentMesh(CreateMesh(mesh, aiScene));
        child->AssignComponent(compMesh);
        compMesh->GetMesh()->totalFaces = mesh->mNumFaces;

        // Create a ComponentTransform and fill it
        ComponentTransform* compTrans = new ComponentTransform();
        this->GetTransformationFromNode(aiNode, compTrans);
        compTrans->CalculateLocal();
        child->AssignComponent(compTrans);
        
        // Create a ComponentMaterial and fill it
        
        ComponentMaterial* compMaterial = new ComponentMaterial();
        child->AssignComponent(compMaterial);
        compMaterial->PassTextures(compMesh->GetMesh()->textures);
        compMaterial->m_MaterialName = aiScene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str();
      /*  meshesList.push_back(CreateMesh(mesh, aiScene));*/
        ++meshesSize;
    }
    if (child == nullptr)
        child = parent;
    // after we've processed all of the meshesList (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        ProcessNode(aiNode->mChildren[i], aiScene, child);
    }

}

// Scene is always the same, <aiScene> contains arrays of data for which <aiMesh> contains indices for those arrays
Mesh* ModelLoader::CreateMesh(aiMesh* aiMesh, const aiScene* aiScene) 
{
    // data to fill
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    for (GLuint i = 0; i < aiMesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vec3; 
        
        // positions
        vec3.x = aiMesh->mVertices[i].x;
        vec3.y = aiMesh->mVertices[i].y;
        vec3.z = aiMesh->mVertices[i].z;
        vertex.m_Position = vec3;

        // normals
        if (aiMesh->HasNormals())
        {
            vec3.x = aiMesh->mNormals[i].x;
            vec3.y = aiMesh->mNormals[i].y;
            vec3.z = aiMesh->mNormals[i].z;
            vertex.m_Normal = vec3;
        }

        // texture coordinates
        if (aiMesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = aiMesh->mTextureCoords[0][i].x;
            vec.y = aiMesh->mTextureCoords[0][i].y;
            vertex.m_TexCoords = vec;
            // tangent
            vec3.x = aiMesh->mTangents[i].x;
            vec3.y = aiMesh->mTangents[i].y;
            vec3.z = aiMesh->mTangents[i].z;
            vertex.m_Tangent = vec3;
            // bitangent
            vec3.x = aiMesh->mBitangents[i].x;
            vec3.y = aiMesh->mBitangents[i].y;
            vec3.z = aiMesh->mBitangents[i].z;
            vertex.m_Bitangent = vec3;
        }
        else
            vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // a face is a aiMesh its triangle
    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
    {
        aiFace face = aiMesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vec3
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);

    }


    // process materials
    aiMaterial* material = aiScene->mMaterials[aiMesh->mMaterialIndex];

    // 1. diffuse maps
    vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // new Mesh
    return new Mesh(vertices, indices, textures, aiMesh->mName.C_Str());
}

vector<Texture> ModelLoader::LoadMaterialTextures(aiMaterial* aiMaterial, aiTextureType aiTextureType, string typeName)
{
    vector<Texture> textures;
    LOG("Material %s", aiMaterial->GetName().C_Str());
    for (unsigned int i = 0; i < aiMaterial->GetTextureCount(aiTextureType); i++)
    {
        aiString str;
        aiMaterial->GetTexture(aiTextureType, i, &str);
        LOG("Texture %s", str.C_Str());

        string tmp = str.C_Str();
        tmp = tmp.substr(tmp.find_last_of("/\\") + 1);
        LOG("Debug %s", tmp.c_str());

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            // Check if we have alreday loaded this texture before
            if (std::strcmp(textures_loaded[j].path.data(), tmp.c_str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = LoadTextureFromFile(tmp.c_str(), this->directory, texture.height, texture.width, texture.nComponents);
            texture.type = typeName;    
            texture.path = tmp.c_str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

void ModelLoader::GetTransformationFromNode(aiNode* node, ComponentTransform* trans)
{
    // Assimp generates dummy nodes to store multiple FBX transformations.
    // All those transformations will be collapsed to the first non-dummy node.
    aiVector3D translation, scaling;
    aiQuaternion rotation;
    node->mTransformation.Decompose(scaling, rotation, translation);

    trans->m_Translation = glm::vec3(translation.x, translation.y, translation.z);
    trans->m_Scaling = glm::vec3(scaling.x, scaling.y, scaling.z);
    trans->m_Rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);

    while (IsDummyNode(*node))
    {
        // As dummies will only have one child, selecting the next one to process is easy.
        aiNode* dummy = node->mChildren[0];

        aiVector3D dTranslation, dScaling;
        aiQuaternion dRotation;

        // Getting the Transform stored in the dummy node.
        dummy->mTransformation.Decompose(dScaling, dRotation, dTranslation);

        glm::vec3 dummyPosition = glm::vec3(dTranslation.x, dTranslation.y, dTranslation.z);
        glm::vec3 dummyScaling = glm::vec3(dScaling.x, dScaling.y, dScaling.z);
        glm::quat dummyRotation = glm::quat(dRotation.x, dRotation.y, dRotation.z, dRotation.w);

        // Adding the dummy's Transform to the current one.
        trans->m_Translation += dummyPosition;
        trans->m_Rotation = trans->m_Rotation * dummyRotation;
        trans->m_Scaling = { trans->m_Scaling.x * dummyScaling.x, trans->m_Scaling.y * dummyScaling.y, trans->m_Scaling.z * dummyScaling.z };
    }
}

bool ModelLoader::IsDummyNode(const aiNode& assimpNode)
{
    // All dummy nodes contain the "_$AssimpFbx$_" string and only one child node.
    return (strstr(assimpNode.mName.C_Str(), "_$AssimpFbx$_") != nullptr && assimpNode.mNumChildren == 1);
}

GLuint LoadTextureFromFile(const char* path, const string& directory, uint& height_, uint& width_, uint& nComponents_, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    height_ = height;
    width_ = width;
    nComponents_ = nrComponents;
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
