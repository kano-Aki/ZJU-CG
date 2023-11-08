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
glm::vec3 cameraPos=glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraGaze=glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraUP=glm::vec3(0.0f,1.0f,0.0f);
float deltatime=0;
float lastframe=0;
float fov=45.0f;//视锥的竖直视角
bool firstMouse=true;
double lastX,lastY,yaw,pitch;

void framebuffer_size_callback(GLFWwindow* window, int new_width,int new_height)
{
    glViewport(0, 0, new_width, new_height);
}//窗口大小改变时的回调函数

void process_input(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);//按下esc键时关闭窗口
    float cameraSpeed=2.5f*deltatime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraGaze;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraGaze;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraGaze, cameraUP)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraGaze, cameraUP)) * cameraSpeed;
}
//up应为view坐标系中的+y轴，gaze应为-z轴，glm::LookAt函数要求的gaze则是+z轴，所以与实际相反，需要取反

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = lastX - xpos;
    float yoffset=lastY-ypos;
    //关于这里，因为gaze方向取反了，实际方向为(0,0,1)，想象y变大，方向应往上抬，此时沿x轴顺时针旋转，所以角度应减小(因为逆时针为正)
    //想象x变大，方向应往右转，此时沿y轴顺时针旋转，所以角度应减小(因为逆时针为正)
    //需要根据实际的gaze方向确定谁减谁
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    // front.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
    // front.y = sin(glm::radians(pitch));
    // front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.x = -sin(glm::radians(yaw));
    front.y = cos(glm::radians(yaw))*sin(glm::radians(pitch));
    front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraGaze = glm::normalize(front);
    //为右手系，从轴的正方向看去的逆时针旋转。设初始方向向量为(0,0,1)。下面结果的正负与坐标系有关
    //因为相机是沿着-z方向观察(变换至原点后)，所以pitch是方向向量和x轴决定的平面和xz平面的夹角(绕x轴旋转)，是一个平面角，
    //也就是方向向量的垂直x轴的分量与xz平面的夹角，所以只旋转pitch时，方向向量会变成(0，-sinp，cosp)
    //yaw是方向向量和y轴决定的平面和zy平面的夹角(绕y轴旋转)，所以只旋转yaw时，方向向量会变成(siny，0，cosy)

    //单看pitch，它不会对x造成影响，但是旋转yaw之后，会对旋转pitch造成影响。事实上对于欧拉角，旋转的顺序对于最终结果是有影响的
    //所以需要指定旋转顺序才能得到确定结果，指定顺序后实际上少了一个自由度，所以欧拉角会造成Gimbal Lock问题，有些角度无法得到。
    //unity中顺序一般为yxz。先绕y轴旋转yaw，然后绕x轴旋转pitch，考虑ppt中的变换矩阵，Rx*Ry，最后方向向量应为(siny,-cosy*sinp,cosy*cosp)
    //与教程不一样，但根据矩阵应该就是这样，很奇怪，结果也是对的
    //参考https://blog.csdn.net/Jaihk662/article/details/106519595

    //关于openGL的左手系
    //首先，opengl之所以是“右手坐标系”，是针对模型空间，视图空间来说的，XYZ轴遵循右手坐标系定则;
    //其次，关于NDC(归一化设备空间)空间，其本来意义是，若一个向量经过mvp矩阵转化，并除以第四个分量w后，
    //若它的XYZ分量均处于[-1，+1]之间，那么它属于NDC空间内，进一步说明它是在视椎体内；
    //我想导致NDC左，右手之分的关键在于NDC坐标系的Z分量，这个值代表的是深度值，
    //若没改变参数的情况下，opengl默认在NDC坐标系下，-1是在近裁剪面处，+1是在远裁剪面处，
    //那么想象一下，近处值小，远处值大，Z轴是朝向屏幕里面的，就解释了为什么NDC是左手系的原因。
    //即在NDC里，为了让z与深度值符合，选择左手系
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if(fov <= 1.0f)
    fov = 1.0f;
  if(fov >= 45.0f)
    fov = 45.0f;
}

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location=1) in vec2 aTexCoord;\n"
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//设置鼠标模式为捕获，即隐藏鼠标
    glfwSetCursorPosCallback(window, mouse_callback);//设置移动鼠标的回调函数s
    glfwSetScrollCallback(window, scroll_callback);//设置滚轮回调函数
    // float vertices[]={
    //     0.5f,0.5f,0.0f,     1.0f,0.0f,0.0f,     2.0f,2.0f,
    //     -0.5f,0.5f,0.0f,    0.0f,1.0f,0.0f,     0.0f,2.0f,
    //     -0.5f,-0.5f,0.0f,   0.0f,0.0f,1.0f,     0.0f,0.0f,
    //     0.5f,-0.5f,0.0f,    1.0f,1.0f,1.0f,     2.0f,0.0f
    // };
    
    // unsigned int indices[]={
    //     0,1,2,
    //     0,2,3
    // };
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,//给出的是local坐标系下的坐标
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    ShaderProgram shaderProgram(vertexShaderSource,fragmentShaderSource);
    unsigned int VBO;
    glGenBuffers(1,&VBO);//创建VBO
    // unsigned int EBO;
    // glGenBuffers(1,&EBO);//创建EBO
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);//创建顶点数组对象
    glBindVertexArray(VAO);//绑定顶点数组对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO);//绑定VBO,这个绑定可以理解成选中VBO作为诸多该类型buffer中的当前使用buffer
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);//绑定EBO
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);//将顶点数据复制到缓冲的内存中

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);//解析顶点数据
    //最后一个变量为偏移量，即从缓冲区的第几个数据开始解析。类型为void*，因为偏移量为1代表从第二个数据开始解析，而数据类型各有不同，所以用void*表示
    glEnableVertexAttribArray(0);//启用顶点属性
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));//解析顶点数据
    glEnableVertexAttribArray(1);//启用顶点属性

    unsigned int text;
    glGenTextures(1,&text);
    glBindTexture(GL_TEXTURE_2D,text);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);//设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//设置纹理过滤方式
    int image_width,image_height,image_nrChannels;
    unsigned char *data=stbi_load("C:/D/ZJU/learn/31/cg/lab/src/learn/container.jpg",&image_width,&image_height,&image_nrChannels,0);
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
    data=stbi_load("C:/D/ZJU/learn/31/cg/lab/src/learn/awesomeface.png",&image_width,&image_height,&image_nrChannels,0);
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
    
    glEnable(GL_DEPTH_TEST);//开启深度测试
    while(!glfwWindowShouldClose(window))//渲染循环
    {
        process_input(window);//输入处理

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//渲染处理
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model=glm::mat4(1.0f);
        // model=glm::rotate(model,(float)glfwGetTime() *glm::radians(50.0f),glm::vec3(0.5f,1.0f,0.0f));
        glm::mat4 view=glm::mat4(1.0f);
        view=glm::lookAt(cameraPos,cameraPos+cameraGaze,cameraUP);
        glm::mat4 projection=glm::mat4(1.0f);
        projection=glm::perspective(glm::radians(fov),(float)width/(float)height,0.1f,100.0f);
        //对于竖直视角，角度越大，显示的物体越小，因为摄像机的视野范围变大了，而viewport的范围不变，所以显示的物体就变小了
        //对于宽高比，若变大，则摄像机视野的宽度变大，但是viewport的宽度不变，所以变换回来物体的宽度会被压缩
        // glUniformMatrix4fv(glGetUniformLocation(shaderProgram.program_ID,"model"),1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.program_ID,"view"),1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.program_ID,"projection"),1,GL_FALSE,glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,text);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,text1);

        shaderProgram.use();//使用着色器程序
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {   
            model=glm::mat4(1.0f);  
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        float currentframe=glfwGetTime();
        deltatime=currentframe-lastframe;
        lastframe=currentframe;

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
    return 0;
}