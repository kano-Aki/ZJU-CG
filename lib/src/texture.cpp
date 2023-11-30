#include "my_header/texture.h"

Texture::Texture(const char* image_path,GLenum texture_wrap_s,GLenum texture_wrap_t, GLenum texture_filter_min,GLenum texture_filter_max)
{
    glGenTextures(1, &texture_num);
    glBindTexture(GL_TEXTURE_2D, texture_num);
    //设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap_t);
    //设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_filter_max);
    //加载图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load(image_path, &width, &height, &nrChannels, 0);
    GLenum format;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;//确定通道种类,jpg和png图像的通道不同
    stbi_set_flip_vertically_on_load(true);//翻转y轴，因为图像坐标系的y轴的正方向是朝下的，而openGL是朝上的
    if (data)
    {
        //生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);//生成纹理
        glGenerateMipmap(GL_TEXTURE_2D);//生成多级渐远纹理
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    //释放图片内存
    stbi_image_free(data);
}