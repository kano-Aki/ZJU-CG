//聚光灯光照
#version 330 core
out vec4 FragColor;//给出输出变量
in vec3 normal;
in vec3 FragPos;
in vec2 TexCoord;
struct Material
{
   vec3 ambient;
   sampler2D diffuse;
   sampler2D specular;//表示该发射光的颜色，环境和漫反射一般即为物体颜色，高光一般为白色
   sampler2D emission;//发射光的贴图

   float ambient_energy;
   float diffuse_energy;
   float specular_energy;//物体产生的该反射光相对于入射光的能量比率，其它能量被物体吸收
//事实上ambient_energy*ambient才是ppt公式中的k。ppt中公式个人认为省略了光源的颜色，认为白光，物体颜色乘上光源颜色为反射的最终颜色
   float shininess;//ppt反射公式的指数p，越大高光区域越小、越集中
};

struct Light
{
   vec3 position;//光源位置
   vec3 color;//光源颜色
   float constant;
   float linear;
   float quadratic;//光线能量衰减公式的系数。
//通过这个系数计算衰减幅度，事实上表达的就是ppt公式中的(I/r^2)，表示该光的强度。
   vec3 spotDir;//聚光灯方向
   float cutoff;//聚光灯的截止角度(以cos值表示)
   float outerCutoff;//聚光灯的外圈角度(以cos值表示)
};
uniform Material material;
uniform Light light;
uniform vec3 viewPos;//若在view空间求光照，则无需这个变量，因为就是原点
uniform float martix_move;//用于控制物体自身发光的贴图的移动,移动y
void main()
{
   vec3 norm=normalize(normal);
   vec3 lightDir=normalize(light.position-FragPos);

   float theta=dot(normalize(-light.spotDir),lightDir);
   float epsilon=light.cutoff-light.outerCutoff;
   float intensity=clamp((theta-light.outerCutoff)/epsilon,0.0,1.0);//聚光灯覆盖范围，内部强度为1，外部为0，过渡处插值
//不影响环境光，让它总是有一点光
   float distance=length(light.position-FragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));//衰减

   vec3 ambi_light=material.ambient_energy*vec3(texture(material.diffuse,TexCoord))*light.color*attenuation;//这里认为环境光与漫反射颜色应为相同

   float diff=max(dot(norm,lightDir),0.0);
   vec3 diff_light=diff*material.diffuse_energy*vec3(texture(material.diffuse,TexCoord))*light.color*attenuation*intensity;

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 spec_light=spec*material.specular_energy*vec3(texture(material.specular,TexCoord))*light.color*attenuation*intensity;

//   vec3 emission_light=vec3(texture(material.emission,vec2(TexCoord.x,TexCoord.y+martix_move)));//物体自身发光，不受其它光源影响

   FragColor=vec4(ambi_light+diff_light+spec_light,1.0f);
}
//片段着色器在对每个像素进行操作，顶点着色器则是对每个顶点操作
//这里在世界空间求光照
//注意glm的vec相乘(*)不是点乘，而是对应项相乘得到新的vec