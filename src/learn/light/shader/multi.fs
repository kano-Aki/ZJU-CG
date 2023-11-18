//多光源光照
#version 330 core

struct Material
{
   vec3 ambient_color;
   sampler2D diffuse_color;
   sampler2D specular_color;//表示该发射光的颜色，环境和漫反射一般即为物体颜色，高光一般为白色

   float ambient_energy;
   float diffuse_energy;
   float specular_energy;//物体产生的该反射光相对于入射光的能量比率，其它能量被物体吸收

   float shininess;//ppt反射公式的指数p，越大高光区域越小、越集中
};

struct DirLight//平行光，无衰减
{
   vec3 direction;//光源到物体的向量
   vec3 color;//光源颜色
};

struct PointLight//点光源，存在衰减
{
   vec3 position;//光源位置
   vec3 color;//光源颜色
   
   float constant;
   float linear;
   float quadratic;//光线能量衰减公式的系数。
};

struct SpotLight//聚光灯，存在衰减，存在截止角度
{
    vec3 position;//光源位置
    vec3 color;//光源颜色

    float cutoff;
    float outerCutoff;
    vec3 spotDir;//聚光灯方向

    float constant;
    float linear;
    float quadratic;//光线能量衰减公式的系数。

    sampler2D spotTexture;//聚光灯纹理,相当于聚光灯颜色
};

out vec4 FragColor;//给出输出变量
in vec3 normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec2 spot_texcoord;//聚光灯纹理坐标，在view空间下计算
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[4];
uniform SpotLight spotLight;
uniform vec3 viewPos;//若在view空间求光照，则无需这个变量，因为就是原点

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);//viewDir为片段到观察者向量,normal已标准化
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 FragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 FragPos, vec3 viewDir);
void main()
{
    vec3 norm=normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result=CalcDirLight(dirLight,norm,viewDir);
    for(int i=0;i<4;i++)
    {
        result+=CalcPointLight(pointLights[i],norm,FragPos,viewDir);
    }
    result+=CalcSpotLight(spotLight,norm,FragPos,viewDir);

    FragColor=vec4(result,1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir=normalize(-light.direction);

    vec3 ambient=light.color*material.ambient_energy*texture(material.diffuse_color,TexCoord).rgb;

    float diff=max(dot(normal,lightDir),0.0);
    vec3 diffuse=diff*light.color*material.diffuse_energy*texture(material.diffuse_color,TexCoord).rgb;

    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=spec*light.color*material.specular_energy*texture(material.specular_color,TexCoord).rgb;

    return (ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal,vec3 FragPos, vec3 viewDir)
{
    vec3 lightDir=normalize(light.position-FragPos);

    float distance=length(light.position-FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient=light.color*material.ambient_energy*texture(material.diffuse_color,TexCoord).rgb*attenuation;

    float diff=max(dot(normal,lightDir),0.0);
    vec3 diffuse=diff*light.color*material.diffuse_energy*texture(material.diffuse_color,TexCoord).rgb*attenuation;

    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=spec*light.color*material.specular_energy*texture(material.specular_color,TexCoord).rgb*attenuation;

    return (ambient+diffuse+specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 FragPos, vec3 viewDir)
{
    vec3 lightDir=normalize(light.position-FragPos);

    float distance=length(light.position-FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));//衰减

    float theta=dot(normalize(-light.spotDir),lightDir);
    float epsilon=light.cutoff-light.outerCutoff;
    float intensity=clamp((theta-light.outerCutoff)/epsilon,0.0,1.0);

    vec3 ambient=light.color*material.ambient_energy*texture(material.diffuse_color,TexCoord).rgb*attenuation;

    float diff=max(dot(normal,lightDir),0.0);
    vec3 diffuse=diff*light.color*material.diffuse_energy*texture(material.diffuse_color,TexCoord).rgb*attenuation*intensity;

    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular=spec*light.color*material.specular_energy*texture(material.specular_color,TexCoord).rgb*attenuation*intensity;

    vec3 spotColor=texture(light.spotTexture,spot_texcoord).rgb*intensity*attenuation*1.0*diff;
    //聚光灯纹理产生的光线，为了更清晰，强度先设为1。实际上类似先在聚光灯灯前套上该纹理样式的薄纱，再照射到物体上，产生的光线。
    //个人认为，考虑实际，应该用这个颜色替换光本身的颜色，计算漫反射光等，而非直接叠加到最终结果上。当然直接叠加效果更好
    //还是没想到怎么将纹理坐标限制在聚光的圆内

    return (ambient+diffuse+specular+spotColor);
}