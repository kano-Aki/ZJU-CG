#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
enum{
    jpg,
    png
};

class Texture {
public:
    Texture(char* image_path,int image_format=jpg,GLenum texture_wrap_s=GL_REPEAT,GLenum texture_wrap_t=GL_REPEAT, 
            GLenum texture_filter_min=GL_LINEAR_MIPMAP_LINEAR,GLenum texture_filter_max=GL_LINEAR);
    //默认生成2D纹理
    unsigned int texture_num;
};




#endif