#ifndef OPENGL_PROJECT_MODELLOADER_H
#define OPENGL_PROJECT_MODELLOADER_H

#include "../scene/model/Model.h"
#include "assimp/scene.h"
#include <map>

#define MAX_DEPTH 100

class ModelLoader {
public:
    static ModelLoader *getInstance();
    std::vector<Mesh *> loadMeshes(const std::string &path);

private:
    std::string directory;
    std::map<std::string, Texture *> loadedTextures;
    std::vector<Mesh *> meshes;

    void processNode(aiNode *node, const aiScene *scene, unsigned int depth = 0);
    Mesh *processMesh(aiMesh *mesh, const aiScene *scene);
    Texture * loadMaterialTexture(aiMaterial *mat, aiTextureType type);

    static glm::vec3 aiVector3DToGlm(aiVector3D from);
    static glm::vec2 aiVector3DToGlmVec2(aiVector3D from);
    static ModelLoader *instance;
    ModelLoader() = default;
};


#endif
