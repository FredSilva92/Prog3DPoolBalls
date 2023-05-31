#pragma once

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

class BallMesh {
public:
	unsigned int VBO, VAO, vertexCount;

	BallMesh(const char* filename);
	~BallMesh();
	
};