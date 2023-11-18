#version 330 core
layout (location = 0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexture;
out vec3 normal;
out vec3 FragPos;
out vec2 TexCoord;
out vec2 spot_texcoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() 
{
   gl_Position = projection*view*model*vec4(aPos, 1.0);
   normal = mat3(transpose(inverse(model))) * aNormal;
   FragPos=vec3(model*vec4(aPos,1.0));
   TexCoord=aTexture;
   spot_texcoord=vec2(view*model*vec4(aPos,1.0))/2+vec2(0.5,0.8);
   //根据view坐标得到纹理坐标，因为屏幕中心实际是原点，但显示的应为纹理中心，所以加上0.5，这里调整成(0.5,0.8)只是为了让人物在中心
   //除2，是为了加0.5之后，可以确保坐标值在0-1内，避免拉伸(这里采用的是GL_CLAMP_TO_EDGE方式处理超出部分)，当然会存在一点比例变化问题
}