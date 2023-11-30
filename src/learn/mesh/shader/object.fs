//多光源光照
#version 330 core

struct Material
{
   vec3 ambient_color;
//    sampler2D diffuse_color;
//    sampler2D specular_color;//表示该发射光的颜色，环境和漫反射一般即为物体颜色，高光一般为白色
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_diffuse4;
    sampler2D texture_diffuse5;
    sampler2D texture_diffuse6;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
     sampler2D texture_specular3;
     sampler2D texture_specular4;
    sampler2D texture_specular5;

   vec3 ambient_energy;
   vec3 diffuse_energy;
   vec3 specular_energy;//物体产生的该反射光相对于入射光的能量比率，其它能量被物体吸收

   float shininess;//ppt反射公式的指数p，越大高光区域越小、越集中
};

struct PointLight//点光源，存在衰减
{
   vec3 position;//光源位置
   vec3 color;//光源颜色
   
   float constant;
   float linear;
   float quadratic;//光线能量衰减公式的系数。
};

struct DirLight//平行光，无衰减
{
   vec3 direction;//光源到物体的向量
   vec3 color;//光源颜色
};


out vec4 FragColor;//给出输出变量
in vec3 normal;
in vec3 FragPos;
in vec2 TexCoord;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[2];
uniform vec3 viewPos;//若在view空间求光照，则无需这个变量，因为就是原点

vec3 CalcPointLight_with_texture(PointLight light, vec3 normal,vec3 FragPos, vec3 viewDir,sampler2D text,int type);
vec3 CalcDirLight_with_texture(DirLight light, vec3 normal, vec3 viewDir,sampler2D text,int type);
void main()
{
    vec3 norm=normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    //vec3 result=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_diffuse1,0);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_specular1,1);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_diffuse2,0);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_specular2,1);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_diffuse3,0);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_specular3,1);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_diffuse4,0);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_specular4,1);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_diffuse5,0);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_specular5,1);
    // result+=CalcDirLight_with_texture(dirLight,norm,viewDir,material.texture_diffuse6,0);
    vec3 result=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_diffuse1,0);
    result+=CalcPointLight_with_texture(pointLights[1],norm,FragPos,viewDir,material.texture_diffuse1,0);
    result+=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_specular1,1);
    result+=CalcPointLight_with_texture(pointLights[1],norm,FragPos,viewDir,material.texture_specular1,1);
    // result+=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_specular2,1);
    // result+=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_diffuse3,0);
    // result+=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_specular3,1);
    // result+=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_diffuse4,0);
    // result+=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_specular4,1);
    // result+=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_diffuse5,0);
    // result+=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_specular5,1);
    // result+=CalcPointLight_with_texture(pointLights[0],norm,FragPos,viewDir,material.texture_diffuse6,0);
    FragColor=vec4(result*1.2,1.0f);
    //FragColor=texture(material.texture_diffuse1,TexCoord);
}

vec3 CalcPointLight_with_texture(PointLight light, vec3 normal,vec3 FragPos, vec3 viewDir,sampler2D text,int type)
{
    vec3 lightDir=normalize(light.position-FragPos);

    float distance=length(light.position-FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient=light.color*material.ambient_energy*texture(text,TexCoord).rgb*attenuation;

    //float diff=max(dot(normal,lightDir),0.0);
    float diff=max(dot(normal,lightDir),-dot(normal,lightDir));
    vec3 diffuse=diff*light.color*material.diffuse_energy*texture(text,TexCoord).rgb*attenuation;

    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=spec*light.color*material.specular_energy*texture(text,TexCoord).rgb*attenuation;

    if(type==0) return (ambient+diffuse);
    else if(type==1) return specular;
}//特殊化处理，对于每个纹理计算其产生的光照，type为0表示传入的为漫反射纹理,为1表示为镜面反射纹理


vec3 CalcDirLight_with_texture(DirLight light, vec3 normal, vec3 viewDir,sampler2D text,int type)
{
    vec3 lightDir=normalize(-light.direction);

    vec3 ambient=light.color*material.ambient_energy*texture(text,TexCoord).rgb;

    float diff=max(dot(normal,lightDir),0.0);
    vec3 diffuse=diff*light.color*material.diffuse_energy*texture(text,TexCoord).rgb;

    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=spec*light.color*material.specular_energy*texture(text,TexCoord).rgb;

    if(type==0) return (ambient+diffuse);
    else if(type==1) return specular;
}