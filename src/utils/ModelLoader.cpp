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

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture *> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture *> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture *> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture *> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return new Mesh(vertices, indices, textures);
}

std::vector<Texture *> ModelLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture *> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString textureName;
        mat->GetTexture(type, i, &textureName);

        if (loadedTextures.find(textureName.C_Str()) != loadedTextures.end()) {
            textures.push_back(loadedTextures[textureName.C_Str()]);
            continue;
        }

        std::string path = directory + "/" + textureName.C_Str(); // todo: typeName
        auto *texture = new Texture(path.c_str());

        loadedTextures[textureName.C_Str()] = texture;
        textures.push_back(texture);
    }

    return textures;
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
