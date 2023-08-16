#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

uniform vec3 cameraPos;//视角位置向量
uniform samplerCube skybox;//用于反射的天空盒采样纹理
void main(){

    vec3 viewDir=normalize(Position-cameraPos);
    vec3 normal=normalize(Normal);
    vec3 R=reflect(viewDir,normal);
    FragColor=vec4(texture(skybox,R).rgb,1.0);
}