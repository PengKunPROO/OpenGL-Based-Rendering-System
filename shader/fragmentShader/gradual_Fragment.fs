#version 330 core
out vec4 FragColors;
uniform vec4 ourColor;
uniform sampler2D texturee;


in vec2 TexCod;
void main()
{
FragColors=texture(texturee,TexCod);
}