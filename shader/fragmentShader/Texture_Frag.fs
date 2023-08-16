#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
//Material Sphere Struct
struct Material {
    sampler2D diffuse;
    sampler2D specular;     
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
// texture samplers
uniform vec3 viewPos;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{

	vec3 normal=normalize(Normal);
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 result=CalcDirLight(dirlight,normal,viewDir);
	result+=CalcPointLight(pointlight,normal,FragPos,viewDir);
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), alpha)*vec4(lightColor*objectColor,1.0);
	//ambient
	// vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	//diffuse
	//vec3 norm=normalize(Normal);
	//vec3 lightDir=normalize(-light.direction);
	//vec3 lightDir=normalize(light.position-FragPos);
	//float diff=max(dot(norm,lightDir),0.0);
	//vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb; //计算diff是cos值
	//specular
	//vec3 viewDir=normalize(viewPos-FragPos);
	//vec3 reflectDir=reflect(-lightDir,norm);
	//float spec=pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
	//vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

	//vec3 result=(ambient+diffuse+specular);
	FragColor =vec4(result,1.0);
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
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
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
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}