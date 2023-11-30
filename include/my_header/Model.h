#ifndef MODEL_H
#define MODEL_H
#include"my_header/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include"my_header/texture.h"
class Model 
{
    public:
        /*  函数   */
        Model(char *path)
        {
            loadModel(path);
        }
        void Draw(ShaderProgram& shader);   
    private:
        /*  模型数据  */
        std::vector<Mesh> meshes;//一个模型(场景)可能有多个网格。每个网格是一个物体。
        std::string directory;
        std::vector<struct texture> textures_loaded;	//不同mesh可能用到同一个纹理，确保纹理不会被加载多次，提高性能
        /*  函数   */
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        unsigned int TextureFromFile(const char *path, const std::string &directory);
};

#endif
/*教程里这个代码要加载其他obj模型必须要带有相应的mtl文件，而且obj模型的所有模型部件都必须至少有一个漫反射贴图，
也就是说，这份代码不支持没有贴图的模型（哪怕是只有一小部分没有贴图），网上很多的模型，比如一个汽车的模型，汽车是黑色的，
通常建模者会给整个车体指定一种黑色高光材质，而没有贴图（这样是为了节约资源，如果想要表现车身上的磨损生锈这样的细节效果，通常会做一个贴图），
然而代码中没有考虑这样的情况，所以程序很可能会在读取模型processMesh函数那里崩溃。所以要读取这样的模型你可能要稍微修改一下代码，
考虑没有贴图的情况（但是mtl文件还是得有，里面存有材质信息，obj本身没有材质信息；
当然你还可以修改代码考虑没有mtl文件的情况下加载一个默认的材质），但是对于有贴图的模型，本人测试大部分obj模型还是支持得很好的
至于很多人的纹理加载不出来可能是mtl文件里的贴图路径问题，mtl本身是一个文本文件，可以用记事本打开，
把一些贴图的绝对路径比如C://tex1.jpg这样的修改成 tex1.jpg这样的相对路径，然后把贴图和模型扔在同一个文件夹里应该就能正常读取了*/