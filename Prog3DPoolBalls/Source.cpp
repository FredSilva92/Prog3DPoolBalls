﻿#pragma region bibliotecas

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
using namespace std;

#include <string>
#include <vector>

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#include "LoadShaders.h"
#include "PoolBalls.h"

#pragma endregion


#pragma region ponto de entrada da aplicação

int main()
{
	// para quando houver algum erro com a glfw
	glfwSetErrorCallback(PoolBalls::printErrorCallback);

	// inicializa a glfw
	if (!glfwInit()) {
		return -1;
	}

	// cria janela
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_NAME, NULL, NULL);
	if (window == nullptr) {
		std::cout << "Erro ao inicializar a biblioteca GLFW" << endl;
		glfwTerminate();
		return -1;
	}

	// define o contexto atual para a renderização da janela
	glfwMakeContextCurrent(window);

	// inicializa a glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Erro ao inicializar a biblioteca GLEW" << endl;
		glfwTerminate();
		return -1;
	}

	// inicializa a cena pela primeira vez
	PoolBalls::init();

	// quando o utilizador faz scroll com o mouse
	glfwSetScrollCallback(window, PoolBalls::scrollCallback);

	// quando o utilizador muda a visão da câmara
	glfwSetCursorPosCallback(window, PoolBalls::mouseCallback);

	// quando é pressionada uma tecla (para ativar/desativar os diferentes tipos de luzes)
	glfwSetCharCallback(window, PoolBalls::charCallback);

	// Main render loop
	while (!glfwWindowShouldClose(window))
	{
		// renderiza os objetos na cena
		PoolBalls::display();

		// troca os buffers de renderização (da frame antiga para a nova)
		glfwSwapBuffers(window);

		// processa todos os eventos ocorridos
		glfwPollEvents();
	}

	// termina todas as instâncias da glfw
	glfwTerminate();

	return 1;
}

#pragma endregion