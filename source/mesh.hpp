#pragma once

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

class Mesh {
    struct VertexPosition3 {glm::vec3 position;};

    struct VertexPosition3Normal3Texture2 {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
    };

    struct VertexPosition2Texture2 {
        glm::vec2 position;
        glm::vec2 textureCoordinates;
    };

public:
    enum VertexType {
        POSITION3,
        POSITION3_NORMAL3_TEXTURE2,
        POSITION2_TEXTURE2
    };

    unsigned int vao{}, vbo{}, ebo{};

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Mesh(const std::vector<float> &vertices = {}, const std::vector<unsigned int> &indices = {},
            const VertexType vertexType = POSITION3_NORMAL3_TEXTURE2): vertices(vertices), indices(indices) {
        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);

        //info // Binding the VBO, VAO and EBO //

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<int>(sizeof(float) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

        glBindVertexArray(vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(sizeof(unsigned int) * indices.size()), indices.data(), GL_STATIC_DRAW);

        //minor // Vertex type //

        if (vertexType == POSITION3) {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosition3), nullptr);
            glEnableVertexAttribArray(0);
        }

        else if (vertexType == POSITION3_NORMAL3_TEXTURE2) {
            //exp // Position attributes //
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosition3Normal3Texture2), reinterpret_cast<void*>(0));
            glEnableVertexAttribArray(0);

            //exp // Normal attributes //
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosition3Normal3Texture2),
                    reinterpret_cast<void*>(offsetof(VertexPosition3Normal3Texture2, normal)));
            glEnableVertexAttribArray(1);

            //exp // Texture attributes //
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosition3Normal3Texture2),
                    reinterpret_cast<void*>(offsetof(VertexPosition3Normal3Texture2, textureCoordinates)));
            glEnableVertexAttribArray(2);
        }

        else if (vertexType == POSITION2_TEXTURE2) {
            //exp // Position attributes //
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosition2Texture2), reinterpret_cast<void*>(0));
            glEnableVertexAttribArray(0);

            //exp // Texture attributes //
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosition2Texture2),
                    reinterpret_cast<void*>(offsetof(VertexPosition2Texture2, textureCoordinates)));
            glEnableVertexAttribArray(1);
        }

        //info // Unbinding the VBO and VAO //

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void remove() const {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    void draw() const {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
    }

    void updateVertices(const std::vector<float> &newVertices) const {
        if (newVertices == vertices) return;

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<int>(sizeof(float) * newVertices.size()), newVertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void updateIndices(const std::vector<unsigned int> &newIndices) const {
        if (newIndices == indices) return;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(sizeof(unsigned int) * newIndices.size()), newIndices.data(), GL_STATIC_DRAW);
    }
};