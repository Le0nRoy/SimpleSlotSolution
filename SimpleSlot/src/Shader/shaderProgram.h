#pragma once
// GLEW
// ����������� ��������� ������, ����� �������������� ����������� ����������
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

class shaderProgram
{
public:
    // ����������� ��������� � �������� ������
    shaderProgram( const GLchar *vertexPath, const GLchar *fragmentPath );
    // ������������� ���������
    inline void Use( )
    {
        glUseProgram( this->Program );
    }

    inline GLuint getProgram( )
    {
        return Program;
    }

private:
    // ������������� ���������
    GLuint Program;
};
