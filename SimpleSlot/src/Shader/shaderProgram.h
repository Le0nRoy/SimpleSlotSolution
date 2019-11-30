#pragma once
// GLEW
// обязательно указывать дефайн, чтобы использовалась статическая библиотека
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

class shaderProgram
{
public:
    // Конструктор считывает и собирает шейдер
    shaderProgram( const GLchar *vertexPath, const GLchar *fragmentPath );
    // Использование программы
    inline void Use( )
    {
        glUseProgram( this->Program );
    }

    inline GLuint getProgram( )
    {
        return Program;
    }

private:
    // Идентификатор программы
    GLuint Program;
};
