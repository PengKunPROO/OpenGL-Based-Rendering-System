#version 330 core
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
    float shininess;
};
struct DirLight {    //定向光
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {	//点光源
    vec3 position;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;//常数项
    float linear;//一次方项
    float quadratic;//二次方项
};
uniform Material material;
uniform DirLight dirlight;
uniform PointLight pointlight;
uniform float alpha;
uniform vec3 viewPos;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal, vec3 fragPos, vec3 viewDir);
void main(){
    vec3 normal=normalize(Normal);
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 result=CalcDirLight(dirlight,normal,viewDir);
	result+=CalcPointLight(pointlight,normal,FragPos,viewDir);
    FragColor=vec4(result,1.0f);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoord).rgb);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord).rgb);
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoord).rgb);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord).rgb);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}