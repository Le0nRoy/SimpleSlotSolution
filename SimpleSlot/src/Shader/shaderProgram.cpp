#include "shaderProgram.h"

shaderProgram::shaderProgram( const GLchar *vertexPath, const GLchar *fragmentPath )
{
    // 1. Получаем исходный код шейдера из filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // Удостоверимся, что ifstream объекты могут выкидывать исключения
    vShaderFile.exceptions( std::ifstream::badbit );
    fShaderFile.exceptions( std::ifstream::badbit );
    try
    {
        // Открываем файлы
        vShaderFile.open( vertexPath );
        fShaderFile.open( fragmentPath );
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        // Считываем данные в потоки
        vShaderStream << vShaderFile.rdbuf( );
        fShaderStream << fShaderFile.rdbuf( );
        // Закрываем файлы
        vShaderFile.close( );
        fShaderFile.close( );
        // Преобразовываем потоки в массив GLchar
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