#include "ModelLoader.h"
#include "Application.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "ModuleComponentSys.h"
#include "ModulePhysics3D.h"
ModelLoader::ModelLoader(ModuleComponentSys* sys, ModulePhysics3D* physics, bool gamma) : gammaCorrection(gamma)
{
    physics3D = physics;
    componentSystem = sys;
    meshesSize = 0;
    PHYSFS_mount("Assets", "/", 1);
    PHYSFS_mount("Resources", "/", 1);
    checkerTexture = nullptr;

    // if texture hasn't been loaded already, load it
    checkerTexture = new Texture();
    checkerTexture->id = LoadTextureFromFile("CheckersTextureDefault.png", "Resources/Textures", checkerTexture->height, checkerTexture->width, checkerTexture->nComponents);
    checkerTexture->type = "texture_diffuse";
    checkerTexture->path = "Checkers";
    textures_loaded.push_back(*checkerTexture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
}

ModelLoader::~ModelLoader()
{
    componentSystem = nullptr;
    physics3D = nullptr;
    checkerTexture = nullptr;
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
    std::string recomputedPath = path;
    std::replace(recomputedPath.begin(), recomputedPath.end(), '\\', '/');
    directory = recomputedPath.substr(0, recomputedPath.find_last_of('/'));
   
    // process ASSIMP's root aiNode recursively
    ProcessNode(scene->mRootNode, scene, parent, *parent->GetTransform());
}

void ModelLoader::LoadTextureIntoGameObject(string const& path, GameObject* go)
{
    std::string recomputedPath = path;
    std::replace(recomputedPath.begin(), recomputedPath.end(), '\\', '/');
    directory = recomputedPath.substr(0, recomputedPath.find_last_of('/'));
    if (go->m_HasComponentMaterial)
    {
        ComponentMaterial* mat = go->GetMaterial();

        // For now all drag and drop texture are diffuse
        string typeDefault = "texture_diffuse";
        string tmp = path;
        tmp = tmp.substr(tmp.find_last_of("/\\") + 1);
        LOG("Debug %s", tmp.c_str());

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            // Check if we have alreday loaded this texture before
            if (std::strcmp(textures_loaded[j].path.data(), tmp.c_str()) == 0)
            {
                mat->m_Textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = LoadTextureFromFile(tmp.c_str(), this->directory, texture.height, texture.width, texture.nComponents);
            texture.type = typeDefault;
            texture.path = tmp.c_str();
            mat->m_Textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    
}

void ModelLoader::ProcessNode(aiNode* aiNode, const aiScene* aiScene, GameObject* parent, ComponentTransform parentWorld)
{
    GameObject* child = nullptr;
    Mesh* myMesh = nullptr;

    // We create transforms outside the loop because we also need the dummy nodes 
    // which have some translations, pre-rotations and rotations that are needed
    ComponentTransform transform = ComponentTransform();
    this->GetTransformationFromNode(aiNode, &transform);
    transform.ComposeLocalMatrix();
    transform.SetLocalMatrix(parentWorld.m_LocalMat * transform.m_LocalMat);

    // process each aiMesh located at the current aiNode
    for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
    {
        // the aiNode object only contains indices to index the actual objects in the aiScene. 
        // the aiScene contains all the data, aiNode is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = aiScene->mMeshes[aiNode->mMeshes[i]];
      
        // Create new GameObject
        child = new GameObject(aiNode->mName.C_Str());
        child->SetParent(parent);
        child->GetParent()->SetChild(child);
        // Create a ComponentMesh and fill it
        ComponentMesh* compMesh = new ComponentMesh(CreateMesh(mesh, aiScene));
        child->AssignComponent(compMesh);
        compMesh->GetMesh()->totalFaces = mesh->mNumFaces;

        // Create a ComponentTransform and assign it
        child->AssignComponent(new ComponentTransform(transform));
        child->GenerateBoundingBoxes();
        // Create a ComponentMaterial and fill it
        ComponentMaterial* compMaterial = new ComponentMaterial();
        child->AssignComponent(compMaterial);
        compMaterial->PassTextures(compMesh->GetMesh()->textures);
        compMaterial->m_MaterialName = aiScene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str();
      /*  meshesList.push_back(CreateMesh(mesh, aiScene));*/
        ++meshesSize;

        child->SetPhysBody(physics3D->AddBodyCube(transform.m_LocalMat, 1.0f));
       

        // Save to Custom format
        Save(compMesh->GetMesh(), compMesh->GetMesh()->name.c_str());
        //Load(compMesh->GetMesh(), compMesh->GetMesh()->name.c_str());
    }

    if (child == nullptr)
        child = parent;

    // after we've processed all of the meshesList (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        // Pass the world transform to the child
        ProcessNode(aiNode->mChildren[i], aiScene, child, transform);
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

    // Extra. Checker texture
    textures.push_back(*checkerTexture);
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
}

bool ModelLoader::IsDummyNode(const aiNode& assimpNode)
{
    // All dummy nodes contain the "_$AssimpFbx$_" string and only one child node.
    return (strstr(assimpNode.mName.C_Str(), "_$AssimpFbx$_") != nullptr && assimpNode.mNumChildren == 1);
}

void ModelLoader::Save(const Mesh* mesh, const char* filename)
{
    // Directory
    string path = filename;
    path += ".mesh";
    string altpath = "Resources/Meshes/";
    altpath += path;
    std::ofstream file2;
    file2.open(altpath, std::ios::in | std::ios::trunc | std::ios::binary);
    if (file2.is_open())
    {
        uint num_Indices = mesh->indices.size();
        uint num_Vertices = mesh->vertices.size();
        uint num_Textures = mesh->textures.size();
        uint num_TotalFaces = mesh->totalFaces;

        // Header pre-data
        long int sizeVertex = sizeof(Vertex);
        uint vertices_Size_In_Bytes = num_Vertices * sizeof(Vertex); // Vertex position
        uint indices_Size_In_Bytes = num_Indices * sizeof(GLuint); // Vertex position

        // Header data
        file2.write((char*)&vertices_Size_In_Bytes, sizeof(Vertex)); // Vertex position, we need size to reserve (malloc or resize for std::) later in load
        file2.write((char*)&indices_Size_In_Bytes, sizeof(GLuint)); // Vertex position, we need size to reserve (malloc or resize for std::) later in load

        // Body data
        file2.write((char*)mesh->vertices.data(), vertices_Size_In_Bytes); // Vertex position, the REAL DATA!
        file2.write((char*)mesh->indices.data(), indices_Size_In_Bytes); // Vertex position, the REAL DATA!

        // Extra data (AABB...)
        file2.write((char*)&mesh->aabb, sizeof(AABB)); // Vertex position, the REAL DATA!
        file2.close();
    }
}

Mesh* ModelLoader::LoadFromCustomFormat(const char* filename, Mesh* compareMesh)
{
    // Directory
    string path = "Resources/Meshes/";
    path += filename;
    path += ".mesh";

    std::ifstream file;
    file.open(path, std::ios::binary);
    if (file)
    {
        vector<Texture> textures;

        uint vertices_Size_In_Bytes = 0;
        uint indices_Size_In_Bytes = 0;

        // Read all header data and resize the vector
        file.read((char*)&vertices_Size_In_Bytes, sizeof(Vertex)); // Vertex position data in bytes, we need size to reserve (malloc or resize for std::)
        file.read((char*)&indices_Size_In_Bytes, sizeof(GLuint)); // Vertex position data in bytes, we need size to reserve (malloc or resize for std::)

        int numVertices = vertices_Size_In_Bytes / sizeof(Vertex);
        std::cout << "Vertices size: " << numVertices << std::endl;
        vector<Vertex> vertices(numVertices);
        std::cout << "Vertices vector size: " << vertices.size() << std::endl;

        int numIndices = indices_Size_In_Bytes / sizeof(GLuint);
        std::cout << "indices size: " << numIndices << std::endl;
        vector<GLuint> indices(numIndices);
        std::cout << "indices vector size: " << indices.size() << std::endl;
        bool everythingCorrect = true;
        // Fill the vector with the boty data
        for (int i = 0; i < numVertices; i++)
        {
            file.read((char*)&vertices[i].m_Position, sizeof(glm::vec3));
            file.read((char*)&vertices[i].m_Normal, sizeof(glm::vec3));
            file.read((char*)&vertices[i].m_TexCoords, sizeof(glm::vec2));
            file.read((char*)&vertices[i].m_Tangent, sizeof(glm::vec3));
            file.read((char*)&vertices[i].m_Bitangent, sizeof(glm::vec3));

            //file.read((char*)&vertices[i].m_BoneIDs, sizeof(int));
            //if (vertices[i].m_BoneIDs != mesh->vertices[i].m_BoneIDs)
            //    everythingCorrect = false;

            //file.read((char*)&vertices[i].m_Weights, sizeof(float));
            //if (vertices[i].m_Weights != mesh->vertices[i].m_Weights)
            //    everythingCorrect = false;
            //
        }
        for (int i = 0; i < numIndices; i++)
        {
            file.read((char*)&indices[i], sizeof(GLuint));
        }
            

        if (everythingCorrect == false)
            printf("Not equal!");

        Mesh* mesh = new Mesh(vertices, indices, textures, filename);
        // Read AABB
        file.read((char*)&mesh->aabb, sizeof(AABB));
         /*mesh->vertices = vertices;
        mesh->indices = indices;*/
        file.close();
        return mesh;
    }
    return nullptr;
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
        LOG("Texture failed to load at path : %s", path);
        stbi_image_free(data);
    }

    return textureID;
}
