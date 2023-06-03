#pragma once

#ifndef POOL_BALLS_H
#define POOL_BALLS_H 1

namespace PoolBalls {

#pragma region bibliotecas

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


#pragma region constantes

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_NAME "Bilhar"

#pragma endregion


#pragma region estruturas

	// estrutura para armazenados dados dos ficheiros .mtl
	typedef struct {
		std::string newmtl;
		float ns;
		glm::vec3 ka;
		glm::vec3 kd;
		glm::vec3 ks;
		float ni;
		float d;
		int illum;
		std::string mapKd;
	} Material;

#pragma endregion


#pragma region funções da biblioteca PoolBalls

	void init(void);
	void display(void);
	std::vector<float> load3dModel(const char* objFilename);
	std::string getMtlFromObj(const char* objFilename);
	Material loadMaterial(const char* mtlFilename);
	void loadTextures(std::vector<string> imageFilenames);
	void loadLightingUniforms();
	void loadMaterialUniforms(Material mt, glm::vec3 emissive);

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