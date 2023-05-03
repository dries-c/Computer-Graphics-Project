#ifndef OPENGL_PROJECT_MATERIAL_H
#define OPENGL_PROJECT_MATERIAL_H


#include "texture/Texture.h"

class Material{
public:
    Material(Texture *diffuseMap, Texture *specularMap, Texture *normalMap, Texture *heightMap, float shininess);

    void bind(const Shader &shader);

    ~Material();
private:
    Texture *diffuseMap;
    Texture *specularMap;
    Texture *normalMap;
    Texture *heightMap;
    float shininess;
};


#endif //OPENGL_PROJECT_MATERIAL_H
