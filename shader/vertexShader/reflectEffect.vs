#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    Normal=mat3(transpose(inverse(model)))*aNormal;
    Position=vec3(model*vec4(aPos,1.0));//这个position是要用来作为片段着色器的输入变量的，用来计算反射向量的
    gl_Position=projection*view*model*vec4(aPos,1.0);   
}