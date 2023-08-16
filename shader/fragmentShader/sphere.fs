#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
uniform sampler2D texture1;
void main(){
    FragColor=vec4(texture(texture1,TexCoord).rgb,1.0f);
}