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
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);//构造函数 第一个参数是顶点着色器文件名 以.vs结尾
	Shader() {}
	void Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr); // Note: geometry source code is optional //         第二个参数是片段着色器文件名 以.fs结尾
	Shader& Use();//使用着色器，一般用于对着色器中的属性值进行设置时需要先使用，相当于激活着色器
			   /*uniform 工具函数*/
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