#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct LightSource {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    LightSource base;
    vec3 direction;
};

struct SpotLight {
    LightSource base;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    LightSource base;
    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

#define MAX_SIZE 100

uniform sampler2D texture1;

uniform vec3 cameraPos;
uniform DirectionalLight directionalLight;
uniform int pointLightCount;
uniform PointLight[MAX_SIZE] pointLights;
uniform SpotLight spotLight;
uniform bool spotLightEnabled;
uniform Material material;
uniform bool hit;

vec3 applyDirectionalLight(vec3 viewDir);
vec3 applyPointLights(PointLight light, vec3 viewDir);
vec3 applySpotLight(vec3 viewDir);
vec3 apply(LightSource light, vec3 lightDir, vec3 viewDir);

void main()
{
    vec3 viewDir = normalize(cameraPos - FragPos);

    vec3 result = applyDirectionalLight(viewDir);
    if(spotLightEnabled){
        result += applySpotLight(viewDir);
    }
    for(int i = 0; i < pointLightCount; i++)
    {
        result += applyPointLights(pointLights[i], viewDir);
    }

    if(hit) {
        result *= vec3(1.0f, 0.5f, 0.5f);
    }

	FragColor = vec4(result, 1.0);
}

vec3 apply(LightSource light, vec3 lightDir, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    return (ambient + diffuse + specular) * light.color;
}

vec3 applyDirectionalLight(vec3 viewDir)
{
    return apply(directionalLight.base, normalize(-directionalLight.direction), viewDir);
}

vec3 applyPointLights(PointLight light, vec3 viewDir)
{
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return apply(light.base, normalize(light.position - FragPos), viewDir) * attenuation;
}

vec3 applySpotLight(vec3 viewDir)
{
    float distance = length(spotLight.position - FragPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));

    vec3 lightDir = normalize(spotLight.position - FragPos);
    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    return apply(spotLight.base, lightDir, viewDir) * attenuation * intensity;
}

