#pragma once

#include <vector>

#include <glad/glad.h>

class Mesh {
public:
    unsigned int vao = 0, vbo = 0, ebo = 0;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Mesh(const std::vector<unsigned int> &indices): indices(indices) {
        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);

        //info // Binding the VBO, VAO and EBO //

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<int>(sizeof(float) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

        glBindVertexArray(vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(sizeof(unsigned int) * indices.size()), indices.data(), GL_STATIC_DRAW);

        //exp // Position attributes //
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);

        //exp // Normal attributes //
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        //exp // Color attributes //
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //exp // Texture attributes //
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void*>(9 * sizeof(float)));
        glEnableVertexAttribArray(3);

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
};