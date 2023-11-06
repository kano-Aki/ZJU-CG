#ifndef shaderProgram_h
#define shaderProgram_h
#include<glad/glad.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;
class ShaderProgram
{
public:
    ShaderProgram(const char* vShader,const char* fShader);
    int use();//启用着色器程序
    int setFloat4(const char* name,float value1,float value2,float value3,float value4);//设置uniform变量

    unsigned int program_ID;
};
#endif