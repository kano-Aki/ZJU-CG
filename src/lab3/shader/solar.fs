#version 330 core
    out vec4 FragColor;//给出输出变量
    in vec3 outColor;
    in vec2 TexCoord;

    uniform sampler2D texture1;
    void main()
    {
       FragColor=texture(texture1,TexCoord);
    }