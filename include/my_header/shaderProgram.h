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
#include<vector>
#define PI 3.1415926

class ShaderProgram
{
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    int use();//启用着色器程序
    int setFloat4(const char* name,float value1,float value2,float value3,float value4);//设置uniform变量
    int setMat4(const char* name, glm::mat4 value);//设置uniform变量
    int setVec3(const char* name, glm::vec3 value);//设置uniform变量
    int setFloat(const char* name,float value);//设置uniform变量
    int setInt(const char* name,int value);
    unsigned int program_ID;

    static void sphere(float radius,glm::vec3 center,int precision,std::vector<float>& vertices,std::vector<unsigned int>& indices);
    //用于定义球体的静态函数，参数分别为半径，球心，精度，顶点数组。

private:
    void checkCompileErrors(unsigned int shader, std::string type);
    //检查编译错误的函数
};
#endif