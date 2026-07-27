#pragma once

#include <print>

#include <GL/glew.h>

#include "files.hpp"

class Shader {
public:
    unsigned int shaderProgramId;

    Shader(const std::string &vertexPath, const std::string &fragmentPath) {
        int success;
        char infoLog[512];

        //minor // Vertex shader //

        const std::string vertexShaderSource = fileRead(vertexPath);
        const char *vertexShaderSourceCChar = vertexShaderSource.c_str();

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertexShader, 1, &vertexShaderSourceCChar, nullptr);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::println("Error! Vertex shader compilation error\n{}", infoLog);
        }

        //minor // Fragment shader //

        const std::string fragmentShaderSource = fileRead(fragmentPath);
        const char *fragmentShaderSourceCChar = fragmentShaderSource.c_str();

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShader, 1, &fragmentShaderSourceCChar, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::println("Error! Fragment shader compilation error\n{}", infoLog);
        }

        //minor // Shader program //

        shaderProgramId = glCreateProgram();

        glAttachShader(shaderProgramId, vertexShader);
        glAttachShader(shaderProgramId, fragmentShader);
        glLinkProgram(shaderProgramId);

        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgramId, 512, nullptr, infoLog);
            std::println("Error! Shader program compilation error!");
        }

        //minor // Deleting shaders (as they are now linked to our program) //

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use() const {glUseProgram(shaderProgramId);}
    void remove() const {glDeleteProgram(shaderProgramId);}

    //minor // `setUniform` methods //

    void setUniform(const std::string &name, bool value) const {glUniform1i(glGetUniformLocation(shaderProgramId, name.c_str()), static_cast<int>(value));}
    void setUniform(const std::string &name, int value) const {glUniform1i(glGetUniformLocation(shaderProgramId, name.c_str()), value);}
    void setUniform(const std::string &name, float value) const {glUniform1f(glGetUniformLocation(shaderProgramId, name.c_str()), value);}

    void setUniform(const std::string &name, const glm::vec2 &vector) const {
        glUniform2fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, glm::value_ptr(vector));
    }
    void setUniform(const std::string &name, const glm::vec3 &vector) const {
        glUniform3fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, glm::value_ptr(vector));
    }
    void setUniform(const std::string &name, const glm::vec4 &vector) const {
        glUniform4fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, glm::value_ptr(vector));
    }

    void setUniform(const std::string &name, const glm::mat2 &matrix) const {
        glUniformMatrix2fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void setUniform(const std::string &name, const glm::mat3 &matrix) const {
        glUniformMatrix3fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void setUniform(const std::string &name, const glm::mat4 &matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }
};