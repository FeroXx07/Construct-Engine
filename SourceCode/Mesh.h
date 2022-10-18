#ifndef _MESH_H
#define _MESH_H
#include <gl/glew.h>
#include <string>
#include <vector>
using namespace std;
#include "shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {

    glm::vec3 Position;    // position
    glm::vec3 Normal;      // normal
    glm::vec2 TexCoords;   // texCoords
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
    string path; // we store the path of the texture to compare with other textures
};

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<GLuint> indices;
    vector<Texture>      textures;
    GLuint VAO;

    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
    ~Mesh();
    // render the mesh
    void Draw(Shader& shader);

private:
    // render data 
    GLuint VBO, EBO;

    // initializes all the buffer objects/arrays
    void SetupMesh();
};
#endif