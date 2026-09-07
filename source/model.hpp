#pragma once

#include <vector>
#include <string>
#include <print>
#include <cstring>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include "mesh2.hpp"

class Model {
public:
    explicit Model(const std::string &modelPath) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags &AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::println("Assimp error!\n{}", import.GetErrorString());
            return;
        }
        directory = modelPath.substr(0, modelPath.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void draw(const Shader &shader) const {for (const auto &mesh: meshes) mesh.draw(shader);}

private:
    std::vector<Texture> texturesLoaded;

    std::vector<Mesh> meshes;
    std::string directory;

    void processNode(const aiNode *node, const aiScene *scene) {
        //exp // Process all the node's meshes (if they exist) //
        for (int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        //exp // And do the same thing for each of it's children //
        for (int i = 0; i < node->mNumChildren; i++) processNode(node->mChildren[i], scene);
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex{};

            glm::vec3 vector3;
            vector3.x = mesh->mVertices[i].x;
            vector3.y = mesh->mVertices[i].y;
            vector3.z = mesh->mVertices[i].z;
            vertex.position = vector3;

            if (mesh->HasNormals()) {
                vector3.x = mesh->mNormals[i].x;
                vector3.y = mesh->mNormals[i].y;
                vector3.z = mesh->mNormals[i].z;
                vertex.normal = vector3;
            }

            if (mesh->mTextureCoords[0]) { //exp // Checking if the mesh contains the texture coordinates //
                glm::vec2 vector2;
                vector2.x = mesh->mTextureCoords[0][i].x;
                vector2.y = mesh->mTextureCoords[0][i].y;
                vertex.textureCoordinates = vector2;
            }
            else vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
        }

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "textureDiffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "textureSpecular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        return {vertices, indices, textures};
    }

    std::vector<Texture> loadMaterialTextures(aiMaterial *material, const aiTextureType type, const std::string &typeName) {
        std::vector<Texture> textures;
        for (int i = 0; i < material->GetTextureCount(type); i++) {
            aiString texturePath;
            material->GetTexture(type, i, &texturePath);

            bool skipTexture = false;
            for (const auto &textureLoaded: texturesLoaded) {
                if (std::strcmp(textureLoaded.path.data(), texturePath.C_Str()) == 0) {
                    textures.push_back(textureLoaded);
                    skipTexture = true;
                    break;
                }
            }

            if (!skipTexture) {
                Texture texture;
                texture.id = TextureFromFile(directory + "/" + texturePath.C_Str());
                texture.type = typeName;
                texture.path = texturePath.C_Str();
                textures.push_back(texture);
                texturesLoaded.push_back(texture);
            }
        }

        return textures;
    }

    static unsigned int TextureFromFile(const std::string &texturePath) {
        unsigned int textureId;

        int width, height, numberOfComponents;

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &numberOfComponents, 0);
        if (data) {
            int format;
            if (numberOfComponents == 1) format = GL_RED;
            else if (numberOfComponents == 3) format = GL_RGB;
            else format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else std::println("Failed to load the texture");

        stbi_image_free(data);

        return textureId;
    }
};