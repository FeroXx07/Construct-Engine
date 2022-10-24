#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Model::Model(string const& path, bool gamma) : gammaCorrection(gamma)
{
    meshesSize = 0;
    LoadModelFrom_aiScene(path);
}

Model::~Model()
{
}

void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshesList.size(); i++)
        meshesList[i].Draw(shader);
}

void Model::LoadModelFrom_aiScene(string const& path)
{
    //PHYSFS_File* myModel;
    //char* buffer;
    //myModel = PHYSFS_openRead(path.c_str());
    //if (myModel)
    //{
    //    /*
    //       PHYSFS_sint64 rc;
    //       PHYSFS_readBytes(myModel, buffer, sizeof(buffer));
    //       PHYSFS_ErrorCode err = PHYSFS_getLastErrorCode();
    //       LOG(PHYSFS_getErrorByCode(err));*/
    //    uint ret;

    //    // Check for end-of-file state on a PhysicsFS filehandle.
    //    
    //    if (!PHYSFS_eof(myModel))
    //    {
    //        // Get total length of a file in bytes
    //        PHYSFS_sint64 file_size = PHYSFS_fileLength(myModel);
    //        buffer = new char[file_size];

    //        // Read data from a PhysicsFS firehandle. Returns a number of bytes read.
    //        uint bytes = PHYSFS_readBytes(myModel, buffer, file_size);

    //        if (bytes != file_size)
    //        {
    //            LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());
    //            delete buffer;
    //        }
    //        else
    //            ret = bytes;
    //    }
    //    else
    //        LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());

    //    // Close a PhysicsFS firehandle
    //    PHYSFS_close(myModel);
    //}

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
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* aiNode, const aiScene* aiScene)
{
    // process each aiMesh located at the current aiNode
    for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
    {
        // the aiNode object only contains indices to index the actual objects in the aiScene. 
        // the aiScene contains all the data, aiNode is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = aiScene->mMeshes[aiNode->mMeshes[i]];
        meshesList.push_back(CreateMesh(mesh, aiScene));
        ++meshesSize;
    }
    // after we've processed all of the meshesList (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        ProcessNode(aiNode->mChildren[i], aiScene);
    }

}

// Scene is always the same, <aiScene> contains arrays of data for which <aiMesh> contains indices for those arrays
Mesh Model::CreateMesh(aiMesh* aiMesh, const aiScene* aiScene) 
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
    return Mesh(vertices, indices, textures, aiMesh->mName.C_Str());
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial* aiMaterial, aiTextureType aiTextureType, string typeName)
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
            texture.id = LoadTextureFromFile(tmp.c_str(), this->directory);
            texture.type = typeName;    
            texture.path = tmp.c_str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

GLuint LoadTextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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
