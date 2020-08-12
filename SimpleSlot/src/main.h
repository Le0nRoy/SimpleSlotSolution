#pragma once
/*
*   Простенькая слотовая игра для
*/
// GLEW
// обязательно указывать дефайн, чтобы использовалась статическая библиотека
//#define GLEW_STATIC
//#include <GL/glew.h>
//// GLFW
//#include <GLFW/glfw3.h>

#include "Shader/shaderProgram.h"
#include "TextOnScreen/textOnScreen.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm\ext\matrix_clip_space.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <functional>
#include <ctime>
#include <chrono>
#include <windows.h>

typedef struct Character
{
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

// Function prototypes
void init_glfw( );

void key_callback( GLFWwindow *window, int key, int scancode, int action, int mode );
void mouse_callback( GLFWwindow *window, int button, int action, int mods  );
void start_button_callback( );
void stop_button_callback( );

void create_slots_shaders( GLint &shaderProgram );
void link_shaders( GLint vertexShader, GLint fragmentShader, GLint &shaderProgram );

void create_ascii_tex( std::map<GLchar, Character> &asciiMap );
void RenderText( GLint asciiShader, GLint VAO, std::map<GLchar, Character> &Characters,
    std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color );

void init_text_context( std::map<GLchar, Character> &Characters, GLint &shaderProgram, GLuint &VAO );
void init_slots_context( GLuint &VAO, GLint &shaderProgram );

GLuint get_random_color( );

void draw_loop( GLFWwindow *window );

// Window dimensions
const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

const GLfloat offset = 15.0f / ( HEIGHT / 2 );
const GLfloat leftBound = -1.0f + offset;
const GLfloat rightBound = 1.0f - offset;
const GLfloat upBound = 1.0f - offset;
const GLfloat downBound = -1.0f + offset;

const GLfloat upButtonBound = -( abs( downBound ) - ( abs( upBound ) + abs( downBound ) ) / 3 ) - offset;
const GLfloat rightBoundLeftButton = -offset;
const GLfloat leftBoundRightButton = offset;

const GLfloat downSlotBound = upButtonBound + offset * 2;
const GLfloat rightBoundleftSlot = -( abs( leftBound ) - ( abs( rightBound ) + abs( leftBound ) ) / 3 ) - offset;
const GLfloat leftBoundMiddleSlot = rightBoundleftSlot + offset * 2;
const GLfloat rightBoundMiddleSlot = leftBoundMiddleSlot + ( abs( rightBound ) + abs( leftBound ) ) / 3 - offset;
const GLfloat leftBoundRightSlot = rightBoundMiddleSlot + offset * 2;

// Light blue for slots - 1.0f, 1.0f, 0.8f, 1.0f
const GLfloat colorForSlots[] = { 1.0f, 1.0f, 0.8f, 1.0f };
// Yellow for win-combo - 1.0f, 1.0f, 0.3f, 1.0f
const GLfloat colorForWinCombo[] = { 1.0f, 1.0f, 0.3f, 1.0f };
// Orange for button - 1.0f, 0.5f, 0.2f, 1.0f
const GLfloat colorForButton[] = { 1.0f, 0.5f, 0.2f, 1.0f };

const GLfloat colors[ 10 ][ 4 ] = {
        { colorForSlots[ 0 ], colorForSlots[ 1 ], colorForSlots[ 2 ], colorForSlots[ 3 ] },
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 1.0f, 1.0f },
        { 0.5f, 0.5f, 0.5f, 1.0f },
        { 0.25f, 0.25f, 0.25f, 1.0f }
};

const GLfloat verticesLeftSlot[] = {
leftBound, upBound, 0.0f,
rightBoundleftSlot, upBound, 0.0f,
rightBoundleftSlot, downSlotBound, 0.0f,
leftBound, downSlotBound, 0.0f
};
const GLfloat verticesMiddleSlot[] = {
leftBoundMiddleSlot, upBound, 0.0f,
rightBoundMiddleSlot, upBound, 0.0f,
rightBoundMiddleSlot, downSlotBound, 0.0f,
leftBoundMiddleSlot, downSlotBound, 0.0f
};
const GLfloat verticesRightSlot[] = {
leftBoundRightSlot, upBound, 0.0f,
rightBound, upBound, 0.0f,
rightBound, downSlotBound, 0.0f,
leftBoundRightSlot, downSlotBound, 0.0f
};

const GLfloat verticesRightButton[] = {
leftBoundRightButton, upButtonBound, 0.0f,
rightBound, upButtonBound, 0.0f,
rightBound, downBound, 0.0f,
leftBoundRightButton, downBound, 0.0f
};
const GLfloat verticesLeftButton[] = {
leftBound, upButtonBound, 0.0f,
rightBoundLeftButton, upButtonBound, 0.0f,
rightBoundLeftButton, downBound, 0.0f,
leftBound, downBound, 0.0f
};
