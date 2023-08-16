#pragma once
//#ifndef RESOURCE_MANAGER_H
//#define RESOURCE_MANAGER_H
#include<map>
#include<string>
#include"Shader.h"
#include"Texture.h"
class ResourceManager//单例模式
{
private:
	static ResourceManager* instance;
private:
	ResourceManager();
	~ResourceManager();

private:
	class Deletor {
	public:
		~Deletor() {
			if (ResourceManager::instance != nullptr)
				delete ResourceManager::instance;
		}
	};
	static Deletor deletor;
public:
	static ResourceManager* getInstance() {
		if (instance == nullptr) {
			instance = new ResourceManager();
		}
		return instance;
	}
//--------------------------工具函数----------------------------//
private:
	static std::map<std::string, Shader> Shader_Map;
	static std::map<std::string, Texture> Texture_Map;	
	static Texture loadTextureFromFile(const GLchar* file,std::string name);
	static Texture loadTextureFromFile(const std::vector<std::string>& faces, std::string name);
public:
	// Loads (and generates) a shader from file
	static Shader   LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
	// Retrieves a stored shader
	static Shader   GetShader(std::string name);
	// Loads (and generates) a texture from file
	static Texture LoadTexture2D(const GLchar* file,std::string name);
	//Loads(and generates) a cube texture from file
	static Texture LoadCubeTexture(std::vector<std::string>& faces,std::string name);
	// Retrieves a stored texture
	static Texture GetTexture(std::string name);
	static void Clear();	
};
//#endif

