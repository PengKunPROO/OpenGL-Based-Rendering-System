#pragma once
//#ifndef MODEL_H
//#define MODEL_H
#include<vector>
#include"Texture.h"
#include"Mesh.h"
#include"Shader.h"
#include<string>
#include"OpenGLINC.h"
#include"ResourceManager.h"
using namespace std;
class Model
{
public:
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
	string name;
	bool gammaCorrection;
	Model(string const& path, string const& name,bool gamma = false) :gammaCorrection(gamma) { 
		this->name = name;
		LoadModel(path); }
	void Draw(Shader& shader);

private:
	void LoadModel(string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
//#endif

