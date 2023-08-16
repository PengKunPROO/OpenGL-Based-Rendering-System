#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 Texccood;

out vec2 Texcod;
void main()
{
gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);
Texcod=vec2(Texccood.x,Texccood.y);
}