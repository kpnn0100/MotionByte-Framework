#pragma once
#include "ProgramManager.h"
namespace MotionByte
{
    GLuint ProgramManager::createCompiledProgram(const char* vertex, const char* fragment) {

        GLuint shader_programme = glCreateProgram();

        GLuint vs, tcs, tes, gs, fs;
        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex, NULL);
        glCompileShader(vs);

        glAttachShader(shader_programme, vs);

        fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment, NULL);
        glCompileShader(fs);


        glAttachShader(shader_programme, fs);
        glLinkProgram(shader_programme);
        glDeleteShader(vs);
        glDeleteShader(fs);
        GLint success;
        glGetShaderiv(shader_programme, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader_programme, 512, NULL, infoLog);
            std::cerr << "Shader compilation error: " << infoLog << std::endl;
        }
        return shader_programme;
    }
}