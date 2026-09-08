#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
};

struct NewTexture {
    unsigned int id;
    std::string type;
    std::string path;
};

class NewMesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<NewTexture> textures;

    NewMesh(std::vector<Vertex> verticesIn, std::vector<unsigned int> indicesIn, std::vector<NewTexture> texturesIn):
            vertices(std::move(verticesIn)), indices(std::move(indicesIn)), textures(std::move(texturesIn)) {
        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);

        //info // Binding the VBO, VAO and EBO //

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<int>(sizeof(Vertex) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

        glBindVertexArray(vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(sizeof(unsigned int) * indices.size()), indices.data(), GL_STATIC_DRAW);

        //exp // Position attributes //
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);

        //exp // Normal attributes //
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
        glEnableVertexAttribArray(1);

        //exp // NewTexture attributes //
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, textureCoordinates)));
        glEnableVertexAttribArray(2);

        //info // Unbinding the VBO and VAO //

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void remove() const {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    void draw(const Shader &shader) const {
        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);

            if (textures[i].type == "texture_diffuse") shader.setUniform("uMaterial.diffuse", i);
            else if (textures[i].type == "texture_specular") shader.setUniform("uMaterial.specular", i);

            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

private:
    unsigned int vao{}, vbo{}, ebo{};
};