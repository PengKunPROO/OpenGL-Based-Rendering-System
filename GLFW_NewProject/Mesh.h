#pragma once
#include"OpenGLINC.h"
#include"Texture.h"
#include"Shader.h"
//纹理命名标准：texture_diffuseN,texture_specularN，texture_normalN，texture_heightN
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;//法向量
	glm::vec2 TexCoords;//纹理坐标
	glm::vec3 Tangent;//切线方向
	glm::vec3 Bitangent;//副切线
};
struct Texture_Struct {
	unsigned int id;
	string type;
	string path;
};
class Mesh
{
public:
	// mesh Data
	std::vector<Vertex>vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>textures;
	unsigned int VAO;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);

private:
	unsigned int VBO, EBO;
	void SetUpMesh();//对每个网格进行设置
};


