#pragma once
#include"OpenGLINC.h"
#include"Texture.h"
#include"Shader.h"
//����������׼��texture_diffuseN,texture_specularN��texture_normalN��texture_heightN
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;//������
	glm::vec2 TexCoords;//��������
	glm::vec3 Tangent;//���߷���
	glm::vec3 Bitangent;//������
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
	void SetUpMesh();//��ÿ�������������
};


