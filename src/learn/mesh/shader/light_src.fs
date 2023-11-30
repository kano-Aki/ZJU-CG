#version 330 core
out vec4 FragColor;//给出输出变量
uniform vec3 lightColor;
void main()
{
   FragColor=vec4(lightColor,1.0f);
}