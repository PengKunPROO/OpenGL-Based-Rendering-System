#pragma once
#include"Light.h"
class PointLight:public Light
{
public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position,float constant, float linear, float quadratic) :Light(ambient, diffuse, specular) {
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
		this->position = position;
	}
	 void Use(Shader shader);
private:
	float constant;//常数项
	float linear;//一次方项
	float quadratic;//二次方项

	glm::vec3 position;
public:
	// 通过 Light 继承
	virtual void SetAmbinet(glm::vec3 ambinet) override;
	virtual void SetDiffuse(glm::vec3 diffuse) override;
	virtual void SetSpecular(glm::vec3 specular) override;
	void SetLinear(float linear);
	void SetQuadratic(float quadratic);
	void SetPosition(glm::vec3 position);
	void SetPosition(float x, float y, float z);
};

void PointLight::Use(Shader shader) {
	shader.Use();
	shader.SetVector3f("pointlight.position", this->position);
	shader.SetVector3f("pointlight.ambient", this->ambient);
	shader.SetVector3f("pointlight.diffuse", this->diffuse);
	shader.SetVector3f("pointlight.specular", this->specular);
	shader.SetFloat("pointlight.constant", this->constant);
	shader.SetFloat("pointlight.linear", this->linear);
	shader.SetFloat("pointlight.quadratic", this->quadratic);
}

void PointLight::SetAmbinet(glm::vec3 ambinet)
{
	this->ambient = ambient;
}

void PointLight::SetDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void PointLight::SetSpecular(glm::vec3 specular)
{
	this->specular = specular;
}
void PointLight::SetLinear(float linear) {
	this->linear = linear;
}
void PointLight::SetQuadratic(float quadratic) {
	this->quadratic = quadratic;
}
void PointLight::SetPosition(glm::vec3 position) {
	this->position = position;
}
void PointLight::SetPosition(float x, float y, float z) {
	glm::vec3 position(x, y, z);
	this->position = position;
}
