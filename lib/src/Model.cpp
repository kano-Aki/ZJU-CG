#include"my_header/Model.h"

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);    

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::Draw(ShaderProgram& shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // 处理节点所有的网格（如果有的话）
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));         
    }
    // 接下来对它的子节点重复这一过程
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<struct vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<struct texture> textures;
    struct material nmaterial;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        struct vertex nvertex;
        // 处理顶点位置、法线和纹理坐标
        glm::vec3 vector; 
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z; 
        nvertex.position = vector;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        nvertex.normal = vector;

        if(mesh->mTextureCoords[0]) // 网格是否有纹理坐标？这里只关心第一组纹理坐标。
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            nvertex.texCoords = vec;
        }
        else nvertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(nvertex);
    }
    // 处理索引
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)//每个face是一个primitive，可能是三角形，也可能是四边形
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)//每个face会指定对应点的index
            indices.push_back(face.mIndices[j]);
    }
    // 处理纹理
    if(mesh->mMaterialIndex >= 0)
    {//当前物体可能有多个纹理
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<struct texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<struct texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        aiColor3D ambient_energy, diffuse_energy, specular_energy;
        material->Get(AI_MATKEY_COLOR_AMBIENT,ambient_energy);
        material->Get(AI_MATKEY_COLOR_DIFFUSE,diffuse_energy);
        material->Get(AI_MATKEY_COLOR_SPECULAR,specular_energy);
        material->Get(AI_MATKEY_SHININESS,nmaterial.shininess);//这四个材质参数在这个mtl文件中，各处均相同，作此简单处理
        nmaterial.ambient_energy=glm::vec3(ambient_energy.r,ambient_energy.g,ambient_energy.b);
        nmaterial.diffuse_energy=glm::vec3(diffuse_energy.r,diffuse_energy.g,diffuse_energy.b);
        nmaterial.specular_energy=glm::vec3(specular_energy.r,specular_energy.g,specular_energy.b);
        nmaterial.shininess/=4;
    }


    return Mesh(vertices, indices, textures, nmaterial);
}

std::vector<struct texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<struct texture> textures;
    std::cout<<mat->GetTextureCount(aiTextureType_DIFFUSE)<<" "<<mat->GetTextureCount(aiTextureType_SPECULAR)<<std::endl;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip=false;
        for(unsigned int j=0;j<textures_loaded.size();j++)
        {
            if(std::strcmp(textures_loaded[j].path.c_str(),str.C_Str())==0)//该纹理已经加载过
            {
                textures.push_back(textures_loaded[j]);
                skip=true;
                break;
            }
        }

        if(!skip)
        {
            struct texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = std::string(str.C_Str());
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    Texture texture(filename.c_str());
    return texture.texture_num;
}