#include "main.h"

static bool gameStarted = false;
static bool gameStoped = false;
static time_t curTime;
static double maxTime = 5;

int main( int argc, char **argv )
{
    init_glfw( );

    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    if ( window == nullptr )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        return -1;
    }
    glfwMakeContextCurrent( window );

    glewExperimental = GL_TRUE;
    if ( glewInit( ) != GLEW_OK )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize( window, &width, &height );
    glViewport( 0, 0, width, height );

    glfwSetKeyCallback( window, key_callback );
    glfwSetMouseButtonCallback( window, mouse_callback );

    draw_loop( window );

    glfwTerminate( );
    return 0;
}

// Good
void init_glfw( )
{
    // Инициализация GLFW
    glfwInit( );
    // Задается минимальная требуемая версия OpenGL.
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    // Установка профайла для которого создается контекст
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    // Выключение возможности изменения размера окна
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
}

// Good
void key_callback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
    // и приложение после этого закроется
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, GL_TRUE );
    }
}

// Good
void mouse_callback( GLFWwindow *window, int button, int action, int mods )
{
    if ( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS )
    {
        GLdouble xpos = 0;
        GLdouble ypos = 0;
        glfwGetCursorPos( window, &xpos, &ypos );
        // Normalize cursor position to coordinates of GLEW lib
        xpos = ( xpos / ( WIDTH / 2 ) ) - 1.0;
        ypos = -( ( ypos / ( HEIGHT / 2 ) ) - 1.0 );
        if ( ypos >= downBound && ypos <= upButtonBound )
        {
            if ( xpos >= leftBound && xpos <= rightBoundLeftButton )
            {
                start_button_callback( );
            }
            else if ( xpos >= leftBoundRightButton && xpos <= rightBound )
            {
                stop_button_callback( );
            }
        }
    }
}

void start_button_callback( )
{
    std::cout << "Start button" << std::endl;
    time( &curTime );
    gameStarted = true;
    gameStoped = false;
}

void stop_button_callback( )
{
    std::cout << "Stop button" << std::endl;
    gameStarted = false;
    gameStoped = true;
}

// Good
void create_slots_shaders( GLint &shaderProgram )
{
    std::string vertexShaderSource =
        "#version 330 core"
        "\n"
        "layout( location = 0 ) in vec3 position;"
        "\n"
        "void main( )"
        "{"
        "    gl_Position = vec4( position.x, position.y, position.z, 1.0 );"
        "}";

    // Идентификатор шейдера
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    // Указываем исходник шейдера и компилируем
    // TODO понять почему напрямую взять указатель из строки не получается и необходим лишний указатель
    const GLchar *src = vertexShaderSource.c_str( );
    glShaderSource( vertexShader, 1, &src, NULL );
    glCompileShader( vertexShader );
    // Проверяем, не возникло ли ошибок при компиляции
    GLint success = 0;
    GLchar infoLog[ 512 ] = { '\0' };
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
    if ( success == GL_FALSE )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    // Green for background - 0.1f, 0.8f, 0.1f, 1.0f
    // Light blue for slots - 1.0f, 1.0f, 0.8f, 1.0f
    // Yellow for win-combo - 1.0f, 1.0f, 0.3f, 1.0f
    // Orange for button - 1.0f, 0.5f, 0.2f, 1.0f
    std::string fragmentShaderSource =
        "#version 330 core"
        "\n"
        "uniform vec4 inColor;"
        "out vec4 color;"
        "\n"
        "void main( )"
        "{"
        "    color = inColor.rgba;"
        "}";

    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    src = fragmentShaderSource.c_str( );
    glShaderSource( fragmentShader, 1, &src, NULL );
    glCompileShader( fragmentShader );
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
    if ( success == GL_FALSE )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    link_shaders( vertexShader, fragmentShader, shaderProgram );
}

// Good
void link_shaders( GLint vertexShader, GLint fragmentShader, GLint &shaderProgram )
{
    // Инициализируется программа
    shaderProgram = glCreateProgram( );
    // К программе добавляются шейдеры
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    // Программа линкуется
    glLinkProgram( shaderProgram );
    // Проверяем, не возникло ли ошибок при линковке
    GLint success = 0;
    GLchar infoLog[ 512 ] = { '\0' };
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
    if ( success == GL_FALSE )
    {
        glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
        std::cout << "ERROR::PROGRAMM::SHADERS::LINK_FAILED\n" << infoLog << std::endl;
    }
    // Удаление шейдеров (после линковки они более не нужны сами по себе)
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
}

// Unknown
void create_ascii_tex( std::map<GLchar, Character> &asciiMap )
{
    FT_Library ft;
    if ( FT_Init_FreeType( &ft ) != 0 )
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    if ( FT_New_Face( ft, "../SimpleSlot/res/arial.ttf", 0, &face ) != 0 )
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    // Set font size
    FT_Set_Pixel_Sizes( face, 0, 48 );
    if ( FT_Load_Char( face, 'X', FT_LOAD_RENDER ) != 0 )
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    }

    std::map<GLchar, Character> Characters;

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); // Disable byte-alignment restriction

    for ( GLubyte c = 0; c < 128; c++ )
    {
        // Load character glyph 
        if ( FT_Load_Char( face, c, FT_LOAD_RENDER ) )
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2( face->glyph->bitmap.width, face->glyph->bitmap.rows ),
            glm::ivec2( face->glyph->bitmap_left, face->glyph->bitmap_top ),
            face->glyph->advance.x
        };
        Characters.insert( std::pair<GLchar, Character>( c, character ) );
    }
    /*
    OpenGL requires that textures all have a 4-byte alignment e.g. their size is always a multiple of 4 bytes.
    Normally this won't be a problem since most textures have a width that is a multiple of 4 and/or use 4 bytes per pixel,
    but since we now only use a single byte per pixel they can have any possible width.
    By setting its unpack alignment equal to 1 we ensure there are no alignment issues (which could cause segmentation faults).
    */
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // Clear FT resources once glyphs are done
    FT_Done_Face( face );
    FT_Done_FreeType( ft );

    asciiMap = Characters;
}

// Unknown (compiles)
void create_ascii_shader( GLint &vertexShader, GLint &fragmentShader )
{
    std::string vertexShaderSource =
        "#version 330 core"
        "\n"
        "layout( location = 0 ) in vec4 vertex; // <vec2 pos, vec2 tex>"
        "\n"
        "out vec2 TexCoords;"
        "\n"
        "uniform mat4 projection;"
        "\n"
        "void main( )"
        "{"
        "    gl_Position = projection * vec4( vertex.xy, 0.0, 1.0 );"
        "    TexCoords = vertex.zw;"
        "}";

    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    const GLchar *src = vertexShaderSource.c_str( );
    glShaderSource( vertexShader, 1, &src, NULL );
    glCompileShader( vertexShader );
    GLint success = 0;
    GLchar infoLog[ 512 ] = { '\0' };
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
    if ( success == GL_FALSE )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    std::string fragmentShaderSource =
        "#version 330 core"
        "\n"
        "in vec2 TexCoords;"
        "\n"
        "out vec4 color;"
        "\n"
        "uniform sampler2D text;"
        "\n"
        "uniform vec3 textColor;"
        "\n"
        "void main( )"
        "{"
        "    vec4 sampled = vec4( 1.0, 1.0, 1.0, texture( text, TexCoords ).r );"
        "    color = vec4( textColor, 1.0 ) * sampled;"
        "}";

    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    src = fragmentShaderSource.c_str( );
    glShaderSource( fragmentShader, 1, &src, NULL );
    glCompileShader( fragmentShader );
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
    if ( success == GL_FALSE )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }
}

// Unknown
void RenderText( GLint asciiShader, GLint VAO, std::map<GLchar, Character> &Characters,
    std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color )
{
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Activate corresponding render state	
    glUseProgram( asciiShader );
    glUniform3f( glGetUniformLocation( asciiShader, "textColor" ), color.x, color.y, color.z );
    glActiveTexture( GL_TEXTURE0 );
    glBindVertexArray( VAO );

    GLuint VBO;
    glGenBuffers( 1, &VBO );

    // Iterate through all characters
    std::string::const_iterator c;
    for ( c = text.begin( ); c != text.end( ); c++ )
    {
        Character ch = Characters[ *c ];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - ( ch.Size.y - ch.Bearing.y ) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[ 6 ][ 4 ] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture( GL_TEXTURE_2D, ch.TextureID );
        // Update content of VBO memory
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        // Render quad
        glDrawArrays( GL_TRIANGLES, 0, 6 );
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += ( ch.Advance >> 6 ) *scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray( 0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_BLEND );
}

// Unknown
void init_text_context( std::map<GLchar, Character> &Characters, GLint &shaderProgram, GLuint &VAO )
{
    create_ascii_tex( Characters );

    GLint vertexShader = 0;
    GLint fragmentShader = 0;
    create_ascii_shader( vertexShader, fragmentShader );
    link_shaders( vertexShader, fragmentShader, shaderProgram );

    GLuint VBO;
    glGenBuffers( 1, &VBO );
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 6 * 4, NULL, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( GLfloat ), 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

// Good
void init_slots_context( GLuint &VAO, GLint &shaderProgram )
{
    create_slots_shaders( shaderProgram );

    // Индексы, указывающие в каком порядке отрисовывать вершины
    GLuint indices[] = {  // Помните, что мы начинаем с 0!
        0, 1, 3,   // Первый треугольник
        1, 2, 3    // Второй треугольник
    };

    // Инициализация буфера
    GLuint VBO;
    glGenBuffers( 1, &VBO );
    // Инициализация стейт-машины;
    glGenVertexArrays( 1, &VAO );
    // Инициализация индексов
    GLuint EBO;
    glGenBuffers( 1, &EBO );

    // Привязываемся к стейту
    glBindVertexArray( VAO );
    // Копируем наши вершины в буфер для OpenGL
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( verticesLeftSlot ), verticesLeftSlot, GL_STATIC_DRAW );
    // Копируем наши индексы в в буфер для OpenGL
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
    // Устанавливаем указатели на вершинные атрибуты
    GLuint posIndex = glGetAttribLocation( shaderProgram, "position" );
    GLuint colIndex = glGetUniformLocation( shaderProgram, "inColor" );
    const GLuint posLocation = 0 * sizeof( GLfloat );
    const GLuint vertexOffset = 3 * sizeof( GLfloat );
    glVertexAttribPointer( posIndex, 3, GL_FLOAT, GL_FALSE, vertexOffset, (GLvoid *) posLocation );
    glEnableVertexAttribArray( posIndex );
    // Отвязываем VAO (НЕ EBO)
    glBindVertexArray( 0 );
}

// Good
GLuint get_random_color( )
{
    /*
    std::default_random_engine generator;
    std::uniform_int_distribution<GLuint> distribution( 1, 9 );
    //GLuint color = distribution( generator );
    auto color = std::bind( distribution, generator );*/
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now( );
    std::chrono::system_clock::duration dtn = tp.time_since_epoch( );
    std::random_device::result_type seed = dtn.count( );
    auto color = std::bind( std::uniform_int_distribution<int>( 1, 9 ),
        std::mt19937( seed ) );
    GLuint res = color( );
    return res;
}

void draw_loop( GLFWwindow *window )
{
    GLuint VAO_slot = 0;
    GLint shaderProgram = 0;
    init_slots_context( VAO_slot, shaderProgram );
    /*
    // Prepare text writing
    std::map<GLchar, Character> Characters;
    GLint textShaderProgram = 0;
    GLuint VAO_text = 0;
    init_text_context( Characters, textShaderProgram, VAO_text );*/

    GLuint colIndex = glGetUniformLocation( shaderProgram, "inColor" );

    // Игровой цикл
    GLuint numOfColor[ 3 ] = { 0, 0, 0 };
    while ( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents( );

        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        /* ---------------------Команды отрисовки здесь----------------------- */
        glUseProgram( shaderProgram );
        glBindVertexArray( VAO_slot );

        glUniform4f( colIndex, colorForButton[0], colorForButton[ 1 ], colorForButton[ 2 ], colorForButton[ 3 ] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( verticesLeftButton ), verticesLeftButton, GL_STATIC_DRAW );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        glBufferData( GL_ARRAY_BUFFER, sizeof( verticesRightButton ), verticesRightButton, GL_STATIC_DRAW );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

        if ( gameStarted )
        {
            numOfColor[ 0 ] = get_random_color( );
            Sleep( 10 );
            numOfColor[ 1 ] = get_random_color( );
            Sleep( 10 );
            numOfColor[ 2 ] = get_random_color( );

            double seconds = difftime( time( 0 ), curTime );
            if ( seconds >= maxTime )
            {
                gameStarted = false;
                gameStoped = true;
            }
        }

        glUniform4f( colIndex, colors[ numOfColor[ 0 ] ][ 0 ], colors[ numOfColor[ 0 ] ][ 1 ], colors[ numOfColor[ 0 ] ][ 2 ], colors[ numOfColor[ 0 ] ][ 3 ] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( verticesLeftSlot ), verticesLeftSlot, GL_STATIC_DRAW );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        glUniform4f( colIndex, colors[ numOfColor[ 1 ] ][ 0 ], colors[ numOfColor[ 1 ] ][ 1 ], colors[ numOfColor[ 1 ] ][ 2 ], colors[ numOfColor[ 1 ] ][ 3 ] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( verticesMiddleSlot ), verticesMiddleSlot, GL_STATIC_DRAW );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        glUniform4f( colIndex, colors[ numOfColor[ 2 ] ][ 0 ], colors[ numOfColor[ 2 ] ][ 1 ], colors[ numOfColor[ 2 ] ][ 2 ], colors[ numOfColor[ 2 ] ][ 3 ] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( verticesRightSlot ), verticesRightSlot, GL_STATIC_DRAW );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

        if ( gameStoped )
        {
            gameStoped = false;
            if ( numOfColor[ 0 ] == numOfColor[ 1 ] && numOfColor[ 1 ] == numOfColor[ 2 ] )
            {
                std::cout << "Congrats!!! You winner!!!" << std::endl;
            }
        }

        glBindVertexArray( 0 );

        //RenderText( textShaderProgram, VAO_text, Characters, "Start", leftBound + offset, downBound + offset, 1.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );

        glfwSwapBuffers( window );
        Sleep( 100 );
    }
}
