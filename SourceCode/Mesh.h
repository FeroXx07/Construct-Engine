#ifndef _MESH_H
#define _MESH_H
#include <gl/glew.h>
#include <string>
#include <vector>
using namespace std;
#include "shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {

    // Crucial data
    glm::vec3 Position;    // position
    glm::vec3 Normal;      // normal
    glm::vec2 TexCoords;   // texCoords

    // Data to enchance normal maps and lightning...
    glm::vec3 Tangent;     // tangent
    glm::vec3 Bitangent;   // bitangent

    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    GLuint id;
    string type;
    // Path to compare with other textures
    string path; 
};

class Mesh {
public:
    // Vectors of data
    vector<Vertex>       vertices;
    vector<GLuint> indices;
    vector<Texture>      textures;
    GLuint VAO;

    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, const char* name);
    ~Mesh();
  
    void Draw(Shader& shader);

private:
    // Buffers
    GLuint VBO, EBO;
    string name = " ";
    // Init the buffers once data is loaded
    void GenerateBuffers();
};
#endif