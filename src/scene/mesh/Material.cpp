#include "Material.h"

void Material::bind(const Shader &shader) {
    if (diffuseMap != nullptr) {
        diffuseMap->bind(shader, "material.diffuse", 0);
    }
    if (specularMap != nullptr) {
        specularMap->bind(shader, "material.specular", 1);
    }
    if (normalMap != nullptr) {
        normalMap->bind(shader, "material.normal", 2);
    }
    if (heightMap != nullptr) {
        heightMap->bind(shader, "material.height", 3);
    }
    shader.setFloat("material.shininess", shininess);
}

Material::Material(Texture *diffuseMap, Texture *specularMap, Texture *normalMap, Texture *heightMap, float shininess) : diffuseMap(diffuseMap), specularMap(specularMap), normalMap(normalMap), heightMap(heightMap), shininess(shininess) {}

Material::~Material() {
    delete diffuseMap;
    delete specularMap;
    delete normalMap;
    delete heightMap;
}
