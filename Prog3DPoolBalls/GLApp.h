#pragma once

#ifndef GLAPP
#define GLAPP 1

#pragma region bibliotecas

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#pragma endregion


#pragma region constantes

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_NAME "Bilhar"

#pragma endregion


#pragma region variáveis globais

extern GLuint vbo;
extern GLuint vao;
extern GLuint programa;

extern glm::mat4 Model, View, Projection;
extern glm::mat3 NormalMatrix;
extern GLfloat angle = 0.0f;

extern const GLuint VertexNum = 36;

extern glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
extern float lastX = 0.0f;
extern float lastY = 0.0f;
extern bool firstMouse = true;

extern float zoomLevel = 1.0f;
extern float minZoom = 0.1f;
extern float maxZoom = 10.0f;
extern float zoomSpeed = 0.1f;

#pragma endregion


#pragma region funções

void printErrorCallback(int code, const char* description);
void init(void);
void display(void);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void charCallback(GLFWwindow* window, unsigned int codepoint);

#pragma endregion

#endif