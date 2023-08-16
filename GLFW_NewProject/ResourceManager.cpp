#include"ResourceManager.h"
void texture_cube_auxiliary_function(Texture texture);
std::map<std::string, Texture>       ResourceManager::Texture_Map;
std::map<std::string, Shader>        ResourceManager::Shader_Map;
ResourceManager* ResourceManager::instance;
ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}
//===============统一指定标准 Load_ 用于初始化 Get_ 用于已有的Shader或Texture的获取 统一使用名称 name 来获取===================//
Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name) {
    string vertexCode;
    string fragmentCode;
    string geometryCode;   
    string path_V = "D:\\shader\\vertexShader\\";
    string path_F = "D:\\shader\\fragmentShader\\";
    string path_G = "D:\\shader\\geometryShader\\";
    string vertexPath = path_V + vShaderFile;
    string fragmentPath = path_F + fShaderFile;
    string geometryPath;
    if (gShaderFile != nullptr) {
         geometryPath = path_G + gShaderFile;
    }   
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vertexPath);
        std::ifstream fragmentShaderFile(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();   
    const GLchar* fShaderCode = fragmentCode.c_str();
    const GLchar* gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}
Shader ResourceManager::GetShader(std::string name) {
	return Shader_Map[name];
}
Texture ResourceManager::LoadTexture2D(const GLchar* file, std::string name) {//加载一个纹理，传入参数为图片路径，是否是PNG，纹理名
    Texture_Map[name] = loadTextureFromFile(file,name);
    return Texture_Map[name];
}
Texture ResourceManager::LoadCubeTexture(std::vector<std::string>& faces,std::string name)
{
    Texture_Map[name] = loadTextureFromFile(faces, name);
    return Texture_Map[name];
}
Texture ResourceManager::GetTexture(std::string name) {
	return Texture_Map[name];
}
void ResourceManager::Clear() {
	// (Properly) delete all shaders	
	for (auto iter : Shader_Map)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (auto iter : Texture_Map)		
		glDeleteTextures(1,&iter.second.ID);
}

Texture ResourceManager::loadTextureFromFile(const GLchar* file,std::string name) {
    // Create Texture object
    Texture texture;
    texture.name = name;//设置加载的纹理名字为该texture的名字
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(file, &width, &height, &nrChannels, 0);   
    if (nrChannels == 3) {
        texture.Internal_Format = GL_RGB;
        texture.Image_Format = GL_RGB;
    }
    else if (nrChannels == 4) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    else if (nrChannels == 1) {
        texture.Internal_Format = GL_RED;
        texture.Image_Format = GL_RED;
    }
        
    //cout << file << " is " << nrChannels << endl;
    texture.Generate(width, height, image, TextureType::TEXTURE_2D);//每一个生成的texture都会有属于自己的ID的
    stbi_image_free(image);
    return texture;
}

Texture ResourceManager::loadTextureFromFile(const std::vector<std::string>& faces, std::string name)
{
    std::string directory = "D:/Resource/Pic/skybox/";
    Texture texture;
    texture.name = name;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);    
    
    for (int i = 0; i < faces.size(); i++) {
        unsigned char* image = stbi_load((directory+faces[i]).c_str(), &width, &height, &nrChannels, 0);
        if (nrChannels == 3) {
            texture.Internal_Format = GL_RGB;
            texture.Image_Format = GL_RGB;
        }
        else if (nrChannels == 4) {
            texture.Internal_Format = GL_RGBA;
            texture.Image_Format = GL_RGBA;
        }
        else if (nrChannels == 1) {
            texture.Internal_Format = GL_RED;
            texture.Image_Format = GL_RED;
        }
        texture.Generate(width, height, image, TextureType::TEXTURE_CUBE,i);
    }
    texture_cube_auxiliary_function(texture);
    return texture;
}
void texture_cube_auxiliary_function(Texture texture) {//生成cube的辅助函数
    texture.Bind(TextureType::TEXTURE_CUBE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
