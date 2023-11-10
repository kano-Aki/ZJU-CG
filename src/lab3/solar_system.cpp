#include"my_header/camera.h"
#include"my_header/shaderProgram.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include<math.h>

#include"stb_image/stb_image.h"
#define WIDTH 800
#define HEIGHT 600

#define EARTH_SPEED 60
#define SATURN_SPEED 30//公转角速度
#define EARTH_SELF_SPEED 120

float deltatime=0.0f;
float lastframe=0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec3 outColor;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"                         
    "{\n"
    "   gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   TexCoord= vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";

const char* fragmentShaderSource="#version 330 core\n"
    "out vec4 FragColor;\n"//给出输出变量
    "in vec3 outColor;\n"
    "in vec2 TexCoord;\n"

    "uniform sampler2D texture1;\n"
    "void main()\n"
    "{\n"
    "   FragColor=texture(texture1,TexCoord);\n"
    "}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //以上完成窗口初始化和回调函数设置

    glEnable(GL_DEPTH_TEST);//开启深度测试
    ShaderProgram shaderProgram(vertexShaderSource,fragmentShaderSource);//创建着色器程序
    vector<float> sun,earth,saturn;
    vector<unsigned int> suni,earthi,saturni;
    ShaderProgram::sphere(6.0f,glm::vec3(0.0f,0.0f,-10.0f),100,sun,suni);//创建太阳顶点数组
    ShaderProgram::sphere(1.0f,glm::vec3(10.0f,0.0f,-10.0f),100,earth,earthi);//创建地球顶点数组
    ShaderProgram::sphere(3.0f,glm::vec3(20.0f,0.0f,-10.0f),100,saturn,saturni);//创建土星顶点数组
    float vertices_sun[sun.size()],vertices_earth[earth.size()],vertices_saturn[saturn.size()];
    unsigned int indices_sun[suni.size()],indices_earth[earthi.size()],indices_saturn[saturni.size()];
    std::copy(sun.begin(),sun.end(),vertices_sun);
    std::copy(earth.begin(),earth.end(),vertices_earth);
    std::copy(saturn.begin(),saturn.end(),vertices_saturn);
    std::copy(suni.begin(),suni.end(),indices_sun);
    std::copy(earthi.begin(),earthi.end(),indices_earth);
    std::copy(saturni.begin(),saturni.end(),indices_saturn);

    unsigned int VBO_sun, VAO_sun, EBO_sun;
    glGenVertexArrays(1, &VAO_sun);
    glGenBuffers(1, &VBO_sun);
    glGenBuffers(1, &EBO_sun);
    glBindVertexArray(VAO_sun);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_sun);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_sun);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_sun), vertices_sun, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_sun), indices_sun, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int VBO_earth, VAO_earth, EBO_earth;
    glGenVertexArrays(1, &VAO_earth);
    glGenBuffers(1, &VBO_earth);
    glGenBuffers(1, &EBO_earth);
    glBindVertexArray(VAO_earth);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_earth);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_earth);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_earth), vertices_earth, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_earth), indices_earth, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int VBO_saturn, VAO_saturn, EBO_saturn;
    glGenVertexArrays(1, &VAO_saturn);
    glGenBuffers(1, &VBO_saturn);
    glGenBuffers(1, &EBO_saturn);
    glBindVertexArray(VAO_saturn);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_saturn);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_saturn);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_saturn), vertices_saturn, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_saturn), indices_saturn, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int text_sun;
    glGenTextures(1,&text_sun);
    glBindTexture(GL_TEXTURE_2D,text_sun);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//设置纹理过滤方式
    int image_width,image_height,image_nrChannels;
    unsigned char *data=stbi_load("C:/D/ZJU/learn/31/cg/lab/src/lab3/sun.jpg",&image_width,&image_height,&image_nrChannels,0);
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

    unsigned int text_earth;
    glGenTextures(1,&text_earth);
    glBindTexture(GL_TEXTURE_2D,text_earth);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//设置纹理过滤方式
    data=stbi_load("C:/D/ZJU/learn/31/cg/lab/src/lab3/earth.jpg",&image_width,&image_height,&image_nrChannels,0);
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

    unsigned int text_saturn;
    glGenTextures(1,&text_saturn);
    glBindTexture(GL_TEXTURE_2D,text_saturn);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//设置纹理过滤方式
    data=stbi_load("C:/D/ZJU/learn/31/cg/lab/src/lab3/saturn.jpg",&image_width,&image_height,&image_nrChannels,0);
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

    float earth_angle=0.0f,saturn_angle=0.0f,earth_self_angle=0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltatime = currentFrame - lastframe;
        lastframe = currentFrame;
        earth_angle+=EARTH_SPEED*deltatime;
        saturn_angle+=SATURN_SPEED*deltatime;
        earth_self_angle+=EARTH_SELF_SPEED*deltatime;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        glm::mat4 trans=glm::mat4(1.0f);
        glm::mat4 rota=glm::mat4(1.0f);
        glm::mat4 inv_trans=glm::mat4(1.0f);
        glm::mat4 model=glm::mat4(1.0f);
        glm::mat4 view=camera.get_view_matrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.view_angle), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        shaderProgram.setMat4("model",model);
        shaderProgram.setMat4("view",view);
        shaderProgram.setMat4("projection",projection);

        glBindTexture(GL_TEXTURE_2D,text_sun);
        glBindVertexArray(VAO_sun);
        glDrawElements(GL_TRIANGLES, suni.size(), GL_UNSIGNED_INT, 0);

        trans=glm::translate(trans,glm::vec3(-10.0f,0.0f,10.0f));//先移回原点
        rota=glm::rotate(rota,glm::radians(earth_self_angle),glm::vec3(0.0f,1.0f,0.0f));
        inv_trans=glm::translate(inv_trans,glm::vec3(10.0f,0.0f,-10.0f));//再移回去
        model=inv_trans*rota*trans*model;
        trans=glm::mat4(1.0f);
        rota=glm::mat4(1.0f);
        inv_trans=glm::mat4(1.0f);
        trans=glm::translate(trans,glm::vec3(0.0f,0.0f,10.0f));//先移回原点
        rota=glm::rotate(rota,glm::radians(earth_angle),glm::vec3(0.0f,1.0f,0.0f));
        inv_trans=glm::translate(inv_trans,glm::vec3(0.0f,0.0f,-10.0f));//再移回去
        model=inv_trans*rota*trans*model;
        shaderProgram.setMat4("model",model);
        glBindTexture(GL_TEXTURE_2D,text_earth);
        glBindVertexArray(VAO_earth);
        glDrawElements(GL_TRIANGLES, earthi.size(), GL_UNSIGNED_INT, 0);

        model=glm::mat4(1.0f);
        trans=glm::mat4(1.0f);
        rota=glm::mat4(1.0f);
        inv_trans=glm::mat4(1.0f);
        trans=glm::translate(trans,glm::vec3(0.0f,0.0f,10.0f));//先移回原点
        rota=glm::rotate(rota,glm::radians(saturn_angle),glm::vec3(0.0f,1.0f,0.0f));
        inv_trans=glm::translate(inv_trans,glm::vec3(0.0f,0.0f,-10.0f));//再移回去
        model=inv_trans*rota*trans*model;
        shaderProgram.setMat4("model",model);
        glBindTexture(GL_TEXTURE_2D,text_saturn);
        glBindVertexArray(VAO_saturn);
        glDrawElements(GL_TRIANGLES, saturni.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.keyboard_input(FORWARD, deltatime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.keyboard_input(BACKWARD, deltatime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.keyboard_input(LEFT, deltatime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.keyboard_input(RIGHT, deltatime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.mouse_input(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.mouse_scroll(static_cast<float>(yoffset));
}