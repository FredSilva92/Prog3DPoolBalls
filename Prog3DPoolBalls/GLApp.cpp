#pragma region bibliotecas

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

#define TINYOBJLOADER_IMPLEMENTATION
#include "thirdParty/tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "StbImage.h"

#include "LoadShaders.h"
#include "GLApp.h"
#include "Bilhar.h"

#pragma endregion


#pragma region funções de callbacks para glfw

void init(void) {
	float xCoord = 1.25f;
	float yCoord = 0.25f;
	float zCoord = 1.25f;

	GLfloat parallelepipedVertex[VertexNum * 6] = {
		//*************************************************
		//                       X+ (face #0)
		// ************************************************
		// Primeiro triângulo
		// Posições
		xCoord, -yCoord,  zCoord,		1.0f, 0.0f, 0.0f,
		xCoord, -yCoord, -zCoord,		1.0f, 0.0f, 0.0f,
		xCoord,  yCoord,  zCoord,		1.0f, 0.0f, 0.0f,
		// Segundo triângulo
		// Posições
		xCoord,  yCoord,  zCoord,		1.0f, 0.0f, 0.0f,
		xCoord, -yCoord, -zCoord,		1.0f, 0.0f, 0.0f,
		xCoord,  yCoord, -zCoord,		1.0f, 0.0f, 0.0f,
		// ************************************************
		//                       X- (face #1)
		// ************************************************
		// Primeiro triângulo
		// Posições
		-xCoord, -yCoord, -zCoord,	-1.0f, 0.0f, 0.0f,
		-xCoord, -yCoord,  zCoord,	-1.0f, 0.0f, 0.0f,
		-xCoord,  yCoord, -zCoord,	-1.0f, 0.0f, 0.0f,
		// Segundo triângulo
		// Posições
		-xCoord,  yCoord, -zCoord,	-1.0f, 0.0f, 0.0f,
		-xCoord, -yCoord,  zCoord,	-1.0f, 0.0f, 0.0f,
		-xCoord,  yCoord,  zCoord,	-1.0f, 0.0f, 0.0f,
		// ************************************************
		//                       Y+ (face #2)
		// ************************************************
		// Primeiro triângulo
		// Posições
		-xCoord,  yCoord,  zCoord,	0.0f, 1.0f, 0.0f,
		 xCoord,  yCoord,  zCoord,	0.0f, 1.0f, 0.0f,
		-xCoord,  yCoord, -zCoord,	0.0f, 1.0f, 0.0f,
		// Segundo triângulo
		// Posições
		-xCoord,  yCoord, -zCoord,	0.0f, 1.0f, 0.0f,
		 xCoord,  yCoord,  zCoord,	0.0f, 1.0f, 0.0f,
		 xCoord,  yCoord, -zCoord,	0.0f, 1.0f, 0.0f,
		 // ************************************************
		 //                       Y- (face #3)
		 // ************************************************
		 // Primeiro triângulo
		 // Posições
		 -xCoord, -yCoord, -zCoord,	0.0f, -1.0f, 0.0f,
		  xCoord, -yCoord, -zCoord,	0.0f, -1.0f, 0.0f,
		 -xCoord, -yCoord,  zCoord,	0.0f, -1.0f, 0.0f,
		 // Segundo triângulo
		 // Posições
		 -xCoord, -yCoord,  zCoord,	0.0f, -1.0f, 0.0f,
		  xCoord, -yCoord, -zCoord,	0.0f, -1.0f, 0.0f,
		  xCoord, -yCoord,  zCoord,	0.0f, -1.0f, 0.0f,
		  // ************************************************
		  //                       Z+ (face #4)
		  // ************************************************
		  // Primeiro triângulo
		  // Posições
		  -xCoord, -yCoord, zCoord,	0.0f, 0.0f, 1.0f,
		   xCoord, -yCoord, zCoord,	0.0f, 0.0f, 1.0f,
		  -xCoord,  yCoord, zCoord,	0.0f, 0.0f, 1.0f,
		  // Segundo triângulo
		  // Posições
		  -xCoord,  yCoord, zCoord,	0.0f, 0.0f, 1.0f,
		   xCoord, -yCoord, zCoord,	0.0f, 0.0f, 1.0f,
		   xCoord,  yCoord, zCoord,	0.0f, 0.0f, 1.0f,
		   // ************************************************
		   //                       Z- (face #5)
		   // ************************************************
		   // Primeiro triângulo
		   // Posições
			xCoord, -yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
		   -xCoord, -yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
			xCoord,  yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
			// Segundo triângulo
			// Posições
			 xCoord,  yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
			-xCoord, -yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
			-xCoord,  yCoord, -zCoord,	0.0f, 0.0f, -1.0f

	};

	vector<vector<float>> balls;

	for (int i = 1; i <= 15; i++) {
		string filename = "textures/PoolBalls/Ball" + to_string(i) + ".obj";
		balls.push_back(loadTextures(filename.c_str()));
	}

	glGenVertexArrays(1, &vao);
	// Faz bind do VAO, cujo nome está definido em 'VAO', com o contexto do OpenGL.
	// Um VAO é criado no primero bind que lhe seja feito.
	// Este VAO passa a estar ativo até que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(parallelepipedVertex), parallelepipedVertex, 0);

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "shaders/poolballs.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/poolballs.frag" },
		{ GL_NONE, NULL }
	};

	programa = loadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLint positionId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "position");
	// Obtém a localização do atributo 'vNormal' no 'programa'.
	GLint normalId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "normal");

	glVertexAttribPointer(positionId, 3 /*3 elementos por vértice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, (3 + 3) * sizeof(float) /*stride*/, (void*)0);
	glVertexAttribPointer(normalId, 3 /*3 elementos por vértice*/, GL_FLOAT/*do tipo float*/, GL_TRUE, (3 + 3) * sizeof(float) /*stride*/, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(positionId);
	glEnableVertexAttribArray(normalId);

	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	View = glm::lookAt(
		cameraPosition,	                 // eye (posição da câmara).
		glm::vec3(0.0f, 0.0f, 0.0f),	// center (para onde está a "olhar")
		glm::vec3(0.0f, 1.0f, 0.0f)		// up
	);
	Model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 ModelView = View * Model;
	NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));

	// ****************************************************
	// Uniforms
	// ****************************************************

	// Atribui valor ao uniform Model
	GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(Model));
	// Atribui valor ao uniform View
	GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
	// Atribui valor ao uniform ModelView
	GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
	glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
	// Atribui valor ao uniform Projection
	GLint projectionId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Projection");
	glProgramUniformMatrix4fv(programa, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));
	// Atribui valor ao uniform NormalMatrix
	GLint normalViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
	glProgramUniformMatrix3fv(programa, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void display(void) {

	static const GLfloat black[] = {
	0.0f, 0.0f, 0.0f, 0.0f
	};
	// Limpa o buffer de cor
	glClearBufferfv(GL_COLOR, 0, black);
	// Limpa o buffer de profundidade
	glClear(GL_DEPTH_BUFFER_BIT);

	// Use the shader program
	glUseProgram(programa);

	//   Model = glm::rotate(glm::mat4(1.0f), angle += 0.0002f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	glm::mat4 ModelView = View * Model;
	NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));
	// Atribui valor ao uniform Model
	GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(Model));
	GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
	GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
	glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
	// Atribui valor ao uniform NormalMatrix
	GLint normalViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
	glProgramUniformMatrix3fv(programa, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));



	glBindVertexArray(vao);

	// Draw the cobble
	glDrawArrays(GL_TRIANGLES, 0, VertexNum);
};

vector<float> loadTextures(const char* filename) {
	vector<float> vertices;

	tinyobj::attrib_t attributes;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	string warning, error;

	if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, filename)) {
		cout << warning << error << '\n';
	}

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {

			glm::vec4 pos = {
				attributes.vertices[index.vertex_index],
				attributes.vertices[index.vertex_index + 1],
				attributes.vertices[index.vertex_index + 2],
				1
			};

			glm::vec3 normal = {
				attributes.normals[index.normal_index],
				attributes.normals[index.normal_index + 1],
				attributes.normals[index.normal_index + 2]
			};

			glm::vec2 textCoord = {
				attributes.texcoords[index.texcoord_index],
				attributes.texcoords[index.texcoord_index + 1]
			};

			//pos = preTransform * pos;

			vertices.push_back(pos.x);
			vertices.push_back(pos.y);
			vertices.push_back(pos.z);
			vertices.push_back(normal.x);
			vertices.push_back(normal.y);
			vertices.push_back(normal.z);
			//vertices.push_back(textCoord.x);
			//vertices.push_back(textCoord.y);
		}
	}

	return vertices;
};

#pragma endregion


#pragma region funções de callbacks para a biblioteca glfw

void printErrorCallback(int code, const char* description) {
	cout << description << endl;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Convert the mouse position to normalized device coordinates (NDC)
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// Calculate the zoom factor based on the scroll offset
	float zoomFactor = 1.0f + static_cast<float>(yoffset) * zoomSpeed;

	View = glm::scale(View, glm::vec3(zoomFactor, zoomFactor, 1.0f));

	zoomLevel += yoffset * zoomSpeed;
	zoomLevel = std::max(minZoom, std::min(maxZoom, zoomLevel));
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		return; // Sai da função se o botão do mouse esquerdo não estiver pressionado
	}

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;

	// Aplicar rotação horizontal 
	View = glm::rotate(View, glm::radians(xOffset), glm::vec3(0.0f, 1.0f, 0.0f));

	// Calcular o vetor direito da câmera
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(View[2])));

	// Aplicar rotação vertical 
	View = glm::rotate(View, glm::radians(yOffset), right);
}

void charCallback(GLFWwindow* window, unsigned int codepoint)
{
	// deteta as teclas do teclado
	switch (codepoint)
	{
	case '1':
		std::cout << "Luz ambiente ativada" << std::endl;
		break;
	case '2':
		std::cout << "Luz direcional ativada" << std::endl;
		break;
	case '3':
		std::cout << "Luz pontual ativada" << std::endl;
		break;
	case '4':
		std::cout << "Luz cónica ativada" << std::endl;
		break;
	default:
		break;
	}
}

#pragma endregion