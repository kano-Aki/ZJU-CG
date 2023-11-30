#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include<vector>
#include"my_header/shaderProgram.h"
struct vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};
struct texture
{
    unsigned int id;
    std::string type;
    std::string path;//在Model中用到，存储纹理对模型文件的相对路径。注意某些模型文件定义的会是绝对路径
};
struct material
{
    glm::vec3 ambient_energy;
    glm::vec3 diffuse_energy;
    glm::vec3 specular_energy;
    float shininess;
};
class Mesh
{
public:
    std::vector<struct vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<struct texture> textures;
    struct material material;
    
    Mesh(std::vector<struct vertex> vertices,std::vector<unsigned int> indices,std::vector<struct texture> textures,struct material material);
    void Draw(ShaderProgram& shader);
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif
