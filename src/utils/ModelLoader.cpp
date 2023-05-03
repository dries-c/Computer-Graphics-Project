#include "ModelLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "FileUtils.h"
#include <iostream>
#include <vector>
#include <map>

ModelLoader *ModelLoader::instance = nullptr;

std::vector<Mesh *> ModelLoader::loadMeshes(const std::string &path) {
    meshes.clear();
    directory = path.substr(0, path.find_last_of('/'));
    std::string filePath = RESOURCES_PATH + path;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
            filePath.c_str(),
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
    );
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }

    processNode(scene->mRootNode, scene);
    loadedTextures.clear();

    return meshes;
}

void ModelLoader::processNode(aiNode *node, const aiScene *scene, unsigned int depth) {
    if (depth > MAX_DEPTH) {
        std::cout << "Node: " << node->mName.C_Str() << std::endl;
        std::cout << "Max depth reached" << std::endl;
        return;
    }

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, depth++);
    }
}

Mesh *ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture *> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        vertex.position = aiVector3DToGlm(mesh->mVertices[i]);
        if (mesh->HasNormals()) {
            vertex.normal = aiVector3DToGlm(mesh->mNormals[i]);
        }

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = aiVector3DToGlmVec2(mesh->mTextureCoords[0][i]);
            vertex.tangent = aiVector3DToGlm(mesh->mTangents[i]);
            vertex.bitangent = aiVector3DToGlm(mesh->mBitangents[i]);
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial *aiMaterial = scene->mMaterials[mesh->mMaterialIndex];

    Texture* diffuseMap = loadMaterialTexture(aiMaterial, aiTextureType_DIFFUSE);
    Texture* specularMap = loadMaterialTexture(aiMaterial, aiTextureType_SPECULAR);
    Texture* normalMap = loadMaterialTexture(aiMaterial, aiTextureType_HEIGHT);
    Texture* heightMap = loadMaterialTexture(aiMaterial, aiTextureType_AMBIENT);

    auto* material = new Material(
        diffuseMap,
        specularMap,
        normalMap,
        heightMap,
        32.0f
    );

    return new Mesh(vertices, indices, material);
}

Texture * ModelLoader::loadMaterialTexture(aiMaterial *mat, aiTextureType type) {
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString textureName;
        mat->GetTexture(type, i, &textureName);

        if (loadedTextures.find(textureName.C_Str()) != loadedTextures.end()) {
            return loadedTextures[textureName.C_Str()];
        }

        std::string path = directory + "/" + textureName.C_Str();
        auto *texture = new Texture(path.c_str());

        loadedTextures[textureName.C_Str()] = texture;
        return texture;
    }

    return nullptr;
}

ModelLoader *ModelLoader::getInstance() {
    if (instance == nullptr) {
        instance = new ModelLoader();
    }

    return instance;
}

glm::vec3 ModelLoader::aiVector3DToGlm(aiVector3D from) {
    return {from.x, from.y, from.z};
}

glm::vec2 ModelLoader::aiVector3DToGlmVec2(aiVector3D from) {
    return {from.x, from.y};
}
