#pragma once
#ifndef SHADER_H
#define SHADER_H
#include"OpenGLINC.h"
using namespace std;
class Shader {
private:	
	void    checkCompileErrors(GLuint object, std::string type);
public:
	unsigned int ID;//ShaderProgramID
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);//���캯�� ��һ�������Ƕ�����ɫ���ļ��� ��.vs��β
	Shader() {}
	void Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr); // Note: geometry source code is optional //         �ڶ���������Ƭ����ɫ���ļ��� ��.fs��β
	Shader& Use();//ʹ����ɫ����һ�����ڶ���ɫ���е�����ֵ��������ʱ��Ҫ��ʹ�ã��൱�ڼ�����ɫ��
			   /*uniform ���ߺ���*/
	void    SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
	void    SetInt(const GLchar* name, GLint value, GLboolean useShader = false);
	void    SetInteger(const GLchar* name, GLint value, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);
	void    SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = false);
};
#endif