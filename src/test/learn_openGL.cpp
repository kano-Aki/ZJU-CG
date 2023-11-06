#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "my_header/shaderProgram.h"
#include <iostream>
#include<math.h>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image/stb_image.h"
#define width 800
#define height 600

void framebuffer_size_callback(GLFWwindow* window, int new_width,int new_height)
{
    glViewport(0, 0, new_width, new_height);
}//窗口大小改变时的回调函数

void process_input(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);//按下esc键时关闭窗口
}

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location=1) in vec3 aColor;\n"
    "layout (location=2) in vec2 aTexCoord;\n"
    "out vec3 outColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"                         
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   outColor=aColor;\n"
    "   TexCoord= vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";
//给出版本号和输入变量。layout将location即变量aPos在管线中的位置设为0，将该顶点属性变量的位置值设为0，这个位置值与
//后面解析顶点数据时的位置相对应。意为传入的数据为位置信息。若不用layout设置，则会默认分配，则在解析顶点数据时，
//需要使用glGetAttribLocation函数获取其位置值。glGetAttribLocation(shaderProgram, "aPos")，第一个参数为着色器程序。
//所谓位置值或许可以认为是该变量的index
const char* fragmentShaderSource="#version 330 core\n"
    "out vec4 FragColor;\n"//给出输出变量
    "in vec3 outColor;\n"
    "in vec2 TexCoord;\n"

    "uniform sampler2D texture1;\n"
    "uniform sampler2D texture2;\n"
    "void main()\n"
    "{\n"
    "   FragColor=mix(texture(texture1,TexCoord),texture(texture2,TexCoord),0.2);\n"
    "}\n\0";
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, "FLAG", NULL, NULL);//创建窗口
    glfwMakeContextCurrent(window);//设置为上下文

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);//告诉GLAD用来加载系统相关的OpenGL函数指针地址的函数

    glViewport(0, 0, width, height);//创建视口
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//设置窗口大小改变时的回调函数

    float vertices[]={
        0.5f,0.5f,0.0f,     1.0f,0.0f,0.0f,     2.0f,2.0f,
        -0.5f,0.5f,0.0f,    0.0f,1.0f,0.0f,     0.0f,2.0f,
        -0.5f,-0.5f,0.0f,   0.0f,0.0f,1.0f,     0.0f,0.0f,
        0.5f,-0.5f,0.0f,    1.0f,1.0f,1.0f,     2.0f,0.0f
    };
    
    unsigned int indices[]={
        0,1,2,
        0,2,3
    };

    ShaderProgram shaderProgram(vertexShaderSource,fragmentShaderSource);
    unsigned int VBO;
    glGenBuffers(1,&VBO);//创建VBO
    unsigned int EBO;
    glGenBuffers(1,&EBO);//创建EBO
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);//创建顶点数组对象
    glBindVertexArray(VAO);//绑定顶点数组对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO);//绑定VBO,这个绑定可以理解成选中VBO作为诸多该类型buffer中的当前使用buffer
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);//绑定EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);//解析顶点数据
//最后一个变量为偏移量，即从缓冲区的第几个数据开始解析。类型为void*，因为偏移量为1代表从第二个数据开始解析，而数据类型各有不同，所以用void*表示
    glEnableVertexAttribArray(0);//启用顶点属性
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));//解析顶点数据
    glEnableVertexAttribArray(1);//启用顶点属性
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));//解析顶点数据
    glEnableVertexAttribArray(2);//启用顶点属性

    unsigned int text;
    glGenTextures(1,&text);
    glBindTexture(GL_TEXTURE_2D,text);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//设置纹理过滤方式
    int image_width,image_height,image_nrChannels;
    unsigned char *data=stbi_load("C:/D/ZJU/learn/31/cg/lab/src/test/container.jpg",&image_width,&image_height,&image_nrChannels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image_width,image_height,0,GL_RGB,GL_UNSIGNED_BYTE,data);//生成纹理
        glGenerateMipmap(GL_TEXTURE_2D);//生成多级渐远纹理
    }
    else
    {
        std::cout<<"Failed to load texture"<<std::endl;
    }
    stbi_image_free(data);//释放图像内存

    unsigned int text1;
    glGenTextures(1,&text1);
    glBindTexture(GL_TEXTURE_2D,text1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//设置纹理过滤方式
    stbi_set_flip_vertically_on_load(true);//翻转y轴
    data=stbi_load("C:/D/ZJU/learn/31/cg/lab/src/test/awesomeface.png",&image_width,&image_height,&image_nrChannels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image_width,image_height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);//生成纹理
        glGenerateMipmap(GL_TEXTURE_2D);//生成多级渐远纹理
    }
    else
    {
        std::cout<<"Failed to load texture"<<std::endl;
    }
    stbi_image_free(data);//释放图像内存
    shaderProgram.use();
    glUniform1i(glGetUniformLocation(shaderProgram.program_ID,"texture1"),0);
    glUniform1i(glGetUniformLocation(shaderProgram.program_ID,"texture2"),1);
    
    while(!glfwWindowShouldClose(window))//渲染循环
    {
        process_input(window);//输入处理

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//渲染处理
        glClear(GL_COLOR_BUFFER_BIT);
        // glm::mat4 trans=glm::mat4(1.0f);
        // trans=glm::translate(trans,glm::vec3(0.4f,-0.4f,0.0f));
        // trans=glm::rotate(trans,(float)glfwGetTime(),glm::vec3(0.0f,0.0f,1.0f));
        // //实际是先旋转再平移，与这个顺序相反
        // //float scale=abs(sin(glfwGetTime()));
        // //trans=glm::scale(trans,glm::vec3(scale,scale,scale));
        // glUniformMatrix4fv(glGetUniformLocation(shaderProgram.program_ID,"transform"),1,GL_FALSE,glm::value_ptr(trans));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,text);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,text1);

        shaderProgram.use();//使用着色器程序
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
    return 0;
}