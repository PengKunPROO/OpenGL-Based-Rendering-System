#pragma once
#include"OpenGLINC.h"
#include"Shader.h"
class Light
{	
public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
public:	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	
	virtual void Use(Shader shader)=0;
	virtual void SetAmbinet(glm::vec3 ambinet) = 0;
	virtual void SetDiffuse(glm::vec3 diffuse) = 0;
	virtual void SetSpecular(glm::vec3 specular) = 0;
};
Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

