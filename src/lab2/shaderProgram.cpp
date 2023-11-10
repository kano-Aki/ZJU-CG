#include<my_header/shaderProgram.h>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image/stb_image.h"
ShaderProgram::ShaderProgram(const char* vShader,const char* fShader)
{
    unsigned int vertex,fragment;
    int success;
    char infoLog[512];

    vertex=glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器
    glShaderSource(vertex,1,&vShader,NULL);//将着色器源码附加到着色器对象上
    glCompileShader(vertex);//编译着色器
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment=glCreateShader(GL_FRAGMENT_SHADER);//创建片段着色器
    glShaderSource(fragment,1,&fShader,NULL);//将着色器源码附加到着色器对象上
    glCompileShader(fragment);//编译着色器
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    program_ID=glCreateProgram();//创建着色器程序
    glAttachShader(program_ID,vertex);//将着色器附加到程序上
    glAttachShader(program_ID,fragment);//将着色器附加到程序上
    glLinkProgram(program_ID);//链接着色器程序
    glGetProgramiv(program_ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program_ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertex);//删除着色器
    glDeleteShader(fragment);//删除着色器
}

int ShaderProgram::use()
{
    glUseProgram(program_ID);
    return 0;
}

int ShaderProgram::setFloat4(const char* name,float value1,float value2,float value3,float value4)
{
    glUniform4f(glGetUniformLocation(program_ID,name),value1,value2,value3,value4);
    return 0;
}

int ShaderProgram::setMat4(const char* name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(program_ID,name),1,GL_FALSE,glm::value_ptr(value));
    return 0;
}

void ShaderProgram::sphere(float radius,glm::vec3 center,int precision,vector<float>& vertices,vector<unsigned int>& indices)
{
    for(int i=0;i<precision;i++)
    {
        for(int j=0;j<precision;j++)
        {
            float theta=i*1.0/(precision-1)*2*PI;///方位角，范围是[0, 2π]
            float phi=j*1.0/(precision-1)*PI;//仰角，范围是[0, π]
            float x=radius*sin(phi)*cos(theta)+center.x;
            float y=radius*sin(phi)*sin(theta)+center.y;
            float z=radius*cos(phi)+center.z;
            float u = i*1.0/(precision-1);//纹理坐标，只是一种映射方法
            float v = j*1.0/(precision-1);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }
    for (int i = 0; i < precision; i++)
	{
		for (int j = 0; j < precision; j++)
		{
            int nexti=(i+1)%precision;
            int nextj=(j+1)%precision;
			indices.push_back(i * precision + j);
			indices.push_back(nexti * precision + j);
			indices.push_back(nexti * precision + nextj);
									 
			indices.push_back(i * precision + j);
			indices.push_back(nexti * precision + nextj);
			indices.push_back(i * precision + nextj);
		}//索引，对于每个点，画出它的上下两个三角形.想象一下，这样的两个三角形组成的就是一个矩形
	}
    //最初在68、69两行，因为计算角度时除以precision，导致两个角度都无法达到2π，所以最后一个点和第一个点无法连接，
    //导致运行时的纹理很明显在图片两侧的拼接处出现明显的模糊条，正是因为纹理坐标虽然相同，但因为角度无法到达2π，所以
    //两个纹理坐标一致的点，空间坐标分开了，openGL自动对中间部分进行插值，因为二者纹理相同，所以插值会显示条带(相当于被拉长了)
}//若精度为100，则画100*100个点来描述球体