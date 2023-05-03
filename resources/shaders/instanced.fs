#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// texture sampler
uniform vec3 cameraPos;
uniform sampler2D texture1;
uniform DirectionalLight directionalLight;
uniform Material material;

vec3 applyDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 viewDir = normalize(cameraPos - FragPos);

    vec3 result = applyDirectionalLight(directionalLight, Normal, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 applyDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular);
}