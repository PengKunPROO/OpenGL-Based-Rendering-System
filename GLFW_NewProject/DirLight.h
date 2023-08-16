#pragma once
#include"Light.h"
class DirLight:public Light
{
public:

	DirLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) :Light(ambient, diffuse, specular) {
		this->direction = direction;
	}
	 void Use(Shader shader);
private:
	glm::vec3 direction;
public:
	// Í¨¹ý Light ¼Ì³Ð
	virtual void SetAmbinet(glm::vec3 ambinet) override;
	virtual void SetDiffuse(glm::vec3 diffuse) override;
	virtual void SetSpecular(glm::vec3 specular) override;
	void SetDirection(glm::vec3 direction);
	void SetDirection(float x, float y, float z);
};
void DirLight::Use(Shader shader) {
	shader.Use();
	shader.SetVector3f("dirLight.direction", this->direction);
	shader.SetVector3f("dirLight.ambient", this->ambient);
	shader.SetVector3f("dirLight.diffuse", this->diffuse);
	shader.SetVector3f("dirLight.specular", this->specular);
}

void DirLight::SetAmbinet(glm::vec3 ambinet)
{
	this->ambient = ambient;
}

void DirLight::SetDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void DirLight::SetSpecular(glm::vec3 specular)
{
	this->specular = specular;
}
void DirLight::SetDirection(glm::vec3 direction) {
	this->direction = direction;
}
void DirLight::SetDirection(float x, float y, float z) {
	glm::vec3 direction(x, y, z);
	this->direction = direction;
}

