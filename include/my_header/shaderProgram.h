#ifndef shaderProgram_h
#define shaderProgram_h
#include<glad/glad.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
class ShaderProgram
{
public:
    ShaderProgram(const char* vShader,const char* fShader);
    int use();//启用着色器程序
    int setFloat4(const char* name,float value1,float value2,float value3,float value4);//设置uniform变量
    int setMat4(const char* name, glm::mat4 value);//设置uniform变量
    unsigned int program_ID;
};
#endif