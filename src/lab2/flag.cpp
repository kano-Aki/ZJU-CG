#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include<math.h>
#include<my_header/shaderProgram.h>
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
    "void main()\n"                         
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"//将顶点位置赋值给内置变量gl_Position
    "}\0";

const char* fragmentShaderSource="#version 330 core\n"
    "out vec4 FragColor;\n"//给出输出变量
    "uniform vec4 ourColor;\n"//给出uniform变量
    "void main()\n"
    "{\n"
    "   FragColor=ourColor;\n"
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

    float vertices_quard[]={
        -0.825f,0.8f,0.0f,
        -0.825f,-0.8f,0.0f,
        0.825f,-0.8f,0.0f,
        0.825f,0.8f,0.0f
    };

    float vertices_strip1[]={
        -0.825f,0.4f,0.0f,
        0.825f,0.4f,0.0f,
        0.825f,0.65f,0.0f,
        -0.825f,0.65f,0.0f,
        -0.825f,-0.4f,0.0f,
        0.825f,-0.4f,0.0f,
        0.825f,-0.65f,0.0f,
        -0.825f,-0.65f,0.0f
    };
    unsigned int indices_strip1[]={
        0,1,2,
        0,2,3,
        4,5,6,
        4,6,7
    };

    float vertices_outer[]={
        0.0f,0.0f,0.0f,
        0.224f,0.1725f,0.0f,
        0.15f,0.0f,0.0f,
        0.224f,-0.1725f,0.0f,
        0.075f,-0.1732f,0.0f,
        0.0f,-0.345f,0.0f,
        -0.075f,-0.1732f,0.0f,
        -0.224f,-0.1725f,0.0f,
        -0.15f,0.0f,0.0f,
        -0.224f,0.1725f,0.0f,
        -0.075f,0.1732f,0.0f,
        0.0f,0.345f,0.0f,
        0.075f,0.1732f,0.0f
        //凹陷的六个点，在原图半径为20
    };
    unsigned int indices_outer[]={
        0,1,2,
        0,2,3,
        0,3,4,
        0,4,5,
        0,5,6,
        0,6,7,
        0,7,8,
        0,8,9,
        0,9,10,
        0,10,11,
        0,11,12,
        0,12,1
    };

    float vertices_hole[]={//正六边形原图边长约为13.65
        0.0f,0.0f,0.0f,
        0.10234f,0.0f,0.0f,
        0.05119f,-0.0887f,0.0f,
        -0.05119f,-0.0887f,0.0f,
        -0.10234f,0.0f,0.0f,
        -0.05119f,0.0887f,0.0f,
        0.05119f,0.0887f,0.0f,

        0.0f,0.2364f,0.0f,//小三角形边长约为7.3
        -0.0274f,0.1732f,0.0f,
        0.0274f,0.1732f,0.0f,

        0.0f,-0.2364f,0.0f,//小三角形边长约为7.3
        -0.0274f,-0.1732f,0.0f,
        0.0274f,-0.1732f,0.0f,

        0.1535f,0.1182f,0.0f,
        0.0987f,0.1182f,0.0f,
        0.1261f,0.0632f,0.0f,

        0.1535f,-0.1182f,0.0f,
        0.0987f,-0.1182f,0.0f,
        0.1261f,-0.0632f,0.0f,

        -0.1535f,0.1182f,0.0f,
        -0.0987f,0.1182f,0.0f,
        -0.1261f,0.0632f,0.0f,

        -0.1535f,-0.1182f,0.0f,
        -0.0987f,-0.1182f,0.0f,
        -0.1261f,-0.0632f,0.0f,
    };
    unsigned int indices_hole[]={
        0,1,2,
        0,2,3,
        0,3,4,
        0,4,5,
        0,5,6,
        0,6,1,

        7,8,9,
        10,11,12,
        13,14,15,
        16,17,18,
        19,20,21,
        22,23,24,
    };
    
    ShaderProgram shaderProgram("C:/D/ZJU/learn/31/cg/lab/src/lab2/shader/flag.vs","C:/D/ZJU/learn/31/cg/lab/src/lab2/shader/flag.fs");//创建着色器程序
    unsigned int VBO_quard;
    glGenBuffers(1,&VBO_quard);//创建VBO
    unsigned int EBO_quard;
    glGenBuffers(1,&EBO_quard);//创建EBO
    unsigned int VAO_quard;
    glGenVertexArrays(1,&VAO_quard);//创建顶点数组对象
    glBindVertexArray(VAO_quard);//绑定顶点数组对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO_quard);//绑定VBO,这个绑定可以理解成选中VBO作为诸多该类型buffer中的当前使用buffer
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_quard),vertices_quard,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_quard);//绑定EBO
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices_quard),indices_quard,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);//解析顶点数据
    glEnableVertexAttribArray(0);//启用顶点属性
    glBindBuffer(GL_ARRAY_BUFFER,0);//解绑VBO
    glBindVertexArray(0);//解绑顶点数组对象

    unsigned int VBO_strip1;
    glGenBuffers(1,&VBO_strip1);//创建VBO
    unsigned int EBO_strip1;
    glGenBuffers(1,&EBO_strip1);//创建EBO
    unsigned int VAO_strip1;
    glGenVertexArrays(1,&VAO_strip1);//创建顶点数组对象
    glBindVertexArray(VAO_strip1);//绑定顶点数组对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO_strip1);//绑定VBO,这个绑定可以理解成选中VBO作为诸多该类型buffer中的当前使用buffer
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_strip1),vertices_strip1,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_strip1);//绑定EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices_strip1),indices_strip1,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);//解析顶点数据
    glEnableVertexAttribArray(0);//启用顶点属性
    glBindBuffer(GL_ARRAY_BUFFER,0);//解绑VBO
    glBindVertexArray(0);//解绑顶点数组对象

    unsigned int VBO_hole;
    glGenBuffers(1,&VBO_hole);//创建VBO
    unsigned int EBO_hole;
    glGenBuffers(1,&EBO_hole);//创建EBO
    unsigned int VAO_hole;
    glGenVertexArrays(1,&VAO_hole);//创建顶点数组对象
    glBindVertexArray(VAO_hole);//绑定顶点数组对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO_hole);//绑定VBO,这个绑定可以理解成选中VBO作为诸多该类型buffer中的当前使用buffer
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_hole),vertices_hole,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_hole);//绑定EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices_hole),indices_hole,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);//解析顶点数据
    glEnableVertexAttribArray(0);//启用顶点属性
    glBindBuffer(GL_ARRAY_BUFFER,0);//解绑VBO
    glBindVertexArray(0);//解绑顶点数组对象

    unsigned int VBO_outer;
    glGenBuffers(1,&VBO_outer);//创建VBO
    unsigned int EBO_outer;
    glGenBuffers(1,&EBO_outer);//创建EBO
    unsigned int VAO_outer;
    glGenVertexArrays(1,&VAO_outer);//创建顶点数组对象
    glBindVertexArray(VAO_outer);//绑定顶点数组对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO_outer);//绑定VBO,这个绑定可以理解成选中VBO作为诸多该类型buffer中的当前使用buffer
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_outer),vertices_outer,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_outer);//绑定EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices_outer),indices_outer,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);//解析顶点数据
    glEnableVertexAttribArray(0);//启用顶点属性
    glBindBuffer(GL_ARRAY_BUFFER,0);//解绑VBO
    glBindVertexArray(0);//解绑顶点数组对象
    
    
    while(!glfwWindowShouldClose(window))//渲染循环
    {
        process_input(window);//输入处理

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//背景为白色
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram.program_ID);//使用着色器程序

        shaderProgram.setFloat4("ourColor",0.0f,0.0f,0.0f,1.0f);//black
        glBindVertexArray(VAO_quard);
        glDrawArrays(GL_LINE_LOOP,0,4);

        shaderProgram.setFloat4("ourColor",0.0f,0.2196f,0.7216f,1.0f);//blue
        glBindVertexArray(VAO_outer);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);

        glBindVertexArray(VAO_strip1);
        glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_INT,0);

        shaderProgram.setFloat4("ourColor",1.0f,1.0f,1.0f,1.0f);//white
        glBindVertexArray(VAO_hole);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
    return 0;
}