#include<my_header/shaderProgram.h>

ShaderProgram::ShaderProgram(const char* vShader,const char* fShader)
{
    // ifstream vertex_file,fragment_file;
    // vertex_file.open(vertices_path);
    // fragment_file.open(fragment_path);

    // stringstream vertex_stream,fragment_stream;
    // vertex_stream<<vertex_file.rdbuf();
    // fragment_stream<<fragment_file.rdbuf();

    // string vertex_code,fragment_code;
    // vertex_stream>>vertex_code;
    // fragment_stream>>fragment_code;
    // const char* vShader=vertex_code.c_str();
    // const char* fShader=fragment_code.c_str();
    // vertex_file.close();
    // fragment_file.close();

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