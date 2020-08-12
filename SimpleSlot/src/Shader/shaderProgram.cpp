#include "shaderProgram.h"

shaderProgram::shaderProgram( const GLchar *vertexPath, const GLchar *fragmentPath )
{
    // 1. �������� �������� ��� ������� �� filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // �������������, ��� ifstream ������� ����� ���������� ����������
    vShaderFile.exceptions( std::ifstream::badbit );
    fShaderFile.exceptions( std::ifstream::badbit );
    try
    {
        // ��������� �����
        vShaderFile.open( vertexPath );
        fShaderFile.open( fragmentPath );
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        // ��������� ������ � ������
        vShaderStream << vShaderFile.rdbuf( );
        fShaderStream << fShaderFile.rdbuf( );
        // ��������� �����
        vShaderFile.close( );
        fShaderFile.close( );
        // ��������������� ������ � ������ GLchar
        vertexCode = vShaderStream.str( );
        fragmentCode = fShaderStream.str( );
    }
    catch ( std::ifstream::failure e )
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );
}