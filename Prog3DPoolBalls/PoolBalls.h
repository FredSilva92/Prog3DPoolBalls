#pragma once

#ifndef POOL_BALLS_H
#define POOL_BALLS_H 1

#pragma region importações

#include <iostream>

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#pragma endregion


namespace PoolBalls {

#pragma region constantes

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_NAME "Bilhar"

#pragma endregion


#pragma region classe base da biblioteca

	class Renderer {
	public:
		void Read(const std::string obj_model_filepath);
		void Send();
		void Draw(glm::vec3 position, glm::vec3 orientation);
	};

	// estrutura para armazenados dados dos ficheiros .mtl
	typedef struct {
		float ns;			// expoente especular (brilho do objeto)
		glm::vec3 ka;		// coeficiente de reflexão da luz ambiente
		glm::vec3 kd;		// coeficiente de reflexão da luz difusa
		glm::vec3 ks;		// coeficiente de reflexão da luz especular 
		std::string map_kd;	// nome do ficheiro da imagem de textura
	} Material;

#pragma endregion


#pragma region funções da biblioteca PoolBalls

	void init(void);
	void display(void);
	std::vector<float> load3dModel(const char* objFilename);
	std::string getMtlFromObj(const char* objFilename);
	Material loadMaterial(const char* mtlFilename);
	void loadTextures(std::vector<string> imageFilenames);
	void loadMaterialUniforms(Material mt, glm::vec3 emissive);
	void loadLightingUniforms();

#pragma endregion


#pragma region funções de callbacks para a biblioteca glfw

	void printErrorCallback(int code, const char* description);
	void init(void);
	void display(void);
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	void charCallback(GLFWwindow* window, unsigned int codepoint);

#pragma endregion

}

#endif