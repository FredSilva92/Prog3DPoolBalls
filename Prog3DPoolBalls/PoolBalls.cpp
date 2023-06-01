#pragma region bibliotecas

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
using namespace std;

#include <fstream>
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
#include "PoolBalls.h"

#pragma endregion


#pragma region variáveis globais

// mesa
const GLuint numberOfTableVertices = 36;
GLfloat tableVertices[numberOfTableVertices * 8];
GLuint tableVAO;
GLuint tableVBO;

// bolas
const GLuint numberOfBalls = 15;
GLuint ballsVAOs[numberOfBalls];
GLuint ballsVBOs[numberOfBalls];
vector<vector<float>> ballsVertices;

// shaders
GLuint programShader;

// câmara
glm::mat4 Model, View, Projection;
glm::mat3 NormalMatrix;
GLfloat angle = 0.0f;
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);

// eventos do mouse
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

float zoomLevel = 1.0f;
float minZoom = 0.1f;
float maxZoom = 10.0f;
float zoomSpeed = 0.1f;

#pragma endregion


#pragma region funções da biblioteca PoolBalls

void PoolBalls::init(void) {
	// posição da mesa
	float xCoord = 1.25f;
	float yCoord = 0.25f;
	float zCoord = 1.25f;

	// coordenadas de textura da mesa
	float xTex = 0.0f;
	float yTex = 0.0f;

	// cria atributos dos vértices da mesa
	GLfloat tableVertices[numberOfTableVertices * 8] = {
		//*************************************************
		//                       X+ (face #0)
		// ************************************************
		// Primeiro triângulo
		// Posições						Cores					Coordenadas de textura
		xCoord, -yCoord,  zCoord,		1.0f, 0.0f, 0.0f,		xTex, yTex,
		xCoord, -yCoord, -zCoord,		1.0f, 0.0f, 0.0f,       xTex, yTex,
		xCoord,  yCoord,  zCoord,		1.0f, 0.0f, 0.0f,       xTex, yTex,
		// Segundo triângulo
		xCoord,  yCoord,  zCoord,		1.0f, 0.0f, 0.0f,       xTex, yTex,
		xCoord, -yCoord, -zCoord,		1.0f, 0.0f, 0.0f,       xTex, yTex,
		xCoord,  yCoord, -zCoord,		1.0f, 0.0f, 0.0f,       xTex, yTex,
		// ************************************************
		//                       X- (face #1)
		// ************************************************
		// Primeiro triângulo
		-xCoord, -yCoord, -zCoord,		-1.0f, 0.0f, 0.0f,		xTex, yTex,
		-xCoord, -yCoord,  zCoord,		-1.0f, 0.0f, 0.0f,		xTex, yTex,
		-xCoord,  yCoord, -zCoord,		-1.0f, 0.0f, 0.0f,		xTex, yTex,
		// Segundo triângulo
		-xCoord,  yCoord, -zCoord,		-1.0f, 0.0f, 0.0f,		xTex, yTex,
		-xCoord, -yCoord,  zCoord,		-1.0f, 0.0f, 0.0f,		xTex, yTex,
		-xCoord,  yCoord,  zCoord,		-1.0f, 0.0f, 0.0f,		xTex, yTex,
		// ************************************************
		//                       Y+ (face #2)
		// ************************************************
		// Primeiro triângulo
		-xCoord, yCoord, zCoord,		0.0f, 1.0f, 0.0f,      xTex, yTex,
		xCoord, yCoord, zCoord,			0.0f, 1.0f, 0.0f,      xTex, yTex,
		-xCoord, yCoord, -zCoord,		0.0f, 1.0f, 0.0f,      xTex, yTex,
		// Segundo triângulo
		-xCoord, yCoord, -zCoord,		0.0f, 1.0f, 0.0f,      xTex, yTex,
		xCoord, yCoord, zCoord,			0.0f, 1.0f, 0.0f,      xTex, yTex,
		xCoord, yCoord, -zCoord,		0.0f, 1.0f, 0.0f,      xTex, yTex,
		// ************************************************
		//                       Y- (face #3)
		// ************************************************
		// Primeiro triângulo
		-xCoord, -yCoord, -zCoord,		0.0f, -1.0f, 0.0f,		xTex, yTex,
		xCoord, -yCoord, -zCoord,		0.0f, -1.0f, 0.0f,		xTex, yTex,
		-xCoord, -yCoord, zCoord,		0.0f, -1.0f, 0.0f,		xTex, yTex,
		// Segundo triângulo
		-xCoord, -yCoord, zCoord,		0.0f, -1.0f, 0.0f,		xTex, yTex,
		xCoord, -yCoord, -zCoord,		0.0f, -1.0f, 0.0f,		xTex, yTex,
		xCoord, -yCoord, zCoord,		0.0f, -1.0f, 0.0f,		xTex, yTex,
		// ************************************************
		//                       Z+ (face #4)
		// ************************************************
		// Primeiro triângulo
		-xCoord, -yCoord, zCoord,		0.0f, 0.0f, 1.0f,      xTex, yTex,
		xCoord, -yCoord, zCoord,		0.0f, 0.0f, 1.0f,      xTex, yTex,
		-xCoord, yCoord, zCoord,		0.0f, 0.0f, 1.0f,      xTex, yTex,
		// Segundo triângulo
		-xCoord, yCoord, zCoord,		0.0f, 0.0f, 1.0f,      xTex, yTex,
		xCoord, -yCoord, zCoord,		0.0f, 0.0f, 1.0f,      xTex, yTex,
		xCoord,  yCoord, zCoord,		0.0f, 0.0f, 1.0f,      xTex, yTex,
		// ************************************************
		//                       Z- (face #5)
		// ************************************************
		// Primeiro triângulo
		xCoord, -yCoord, -zCoord,		0.0f, 0.0f, -1.0f,      xTex, yTex,
		-xCoord, -yCoord, -zCoord,		0.0f, 0.0f, -1.0f,      xTex, yTex,
		xCoord, yCoord, -zCoord,		0.0f, 0.0f, -1.0f,      xTex, yTex,
		// Segundo triângulo
		xCoord, yCoord, -zCoord,		0.0f, 0.0f, -1.0f,      xTex, yTex,
		-xCoord, -yCoord, -zCoord,		0.0f, 0.0f, -1.0f,      xTex, yTex,
		-xCoord, yCoord, -zCoord,		0.0f, 0.0f, -1.0f,      xTex, yTex
	};

	// gera o nome para o VAO da mesa
	glGenVertexArrays(1, &tableVAO);

	// vincula o VAO ao contexto OpenGL atual
	glBindVertexArray(tableVAO);

	// gera o nome para o VBO da mesa
	glGenBuffers(1, &tableVBO);

	// vincula o VBO ao contexto OpenGL atual
	glBindBuffer(GL_ARRAY_BUFFER, tableVBO);

	// inicializa o VBO atualmente ativo com dados imutáveis
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(tableVertices), tableVertices, 0);

	// ativar atributos das posições dos vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// ativar atributos das cores dos vértices
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// ativar atributos das coordenadas de textura dos vértices
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// desvincula o VAO atual
	glBindVertexArray(0);

	// carrega os atributos dos vértices de cada bola a partir de ficheiros .obj
	for (int i = 1; i <= numberOfBalls; i++) {
		string filename = "textures/Ball" + to_string(i) + ".obj";
		ballsVertices.push_back(PoolBalls::loadTextures(filename.c_str()));
	}

	// gera nomes para os VAOs das bolas
	glGenVertexArrays(numberOfBalls, ballsVAOs);

	// vincula cada VAO das bolas ao contexto OpenGL atual
	for (int i = 0; i < numberOfBalls; i++) {
		glBindVertexArray(ballsVAOs[i]);
	}

	// gera nomes para os VBOs das bolas
	glGenBuffers(numberOfBalls, ballsVBOs);

	// cria e configurar cada VBO das bolas
	for (int i = 0; i < ballsVertices.size(); i++) {
		// vincula o VBO ao contexto OpenGL atual
		glBindBuffer(GL_ARRAY_BUFFER, ballsVBOs[i]);

		// inicializa o vbo atualmente ativo com dados imutáveis
		glBufferStorage(GL_ARRAY_BUFFER, ballsVertices[i].size() * sizeof(float), ballsVertices[i].data(), 0);

		// ativar atributos das posições dos vértices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		// ativar atributos das cores dos vértices
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// ativar atributos das coordenadas de textura dos vértices
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// desvincula o VAO atual
		glBindVertexArray(0);
	}

	// cria informações dos shaders
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER,   "shaders/poolballs.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/poolballs.frag" },
		{ GL_NONE, NULL }
	};

	// carrega shaders
	programShader = loadShaders(shaders);

	// se houve erros ao carregar shaders
	if (!programShader) {
		cout << "Erro ao carregar shaders: " << endl;
		exit(EXIT_FAILURE);
	}

	// vincula o programa shader ao contexto OpenGL atual
	glUseProgram(programShader);

	// obtém as localizações dos atributos no programa shader
	GLint positionId = glGetProgramResourceLocation(programShader, GL_PROGRAM_INPUT, "position");
	GLint normalId = glGetProgramResourceLocation(programShader, GL_PROGRAM_INPUT, "normal");
	GLint textCoordId = glGetProgramResourceLocation(programShader, GL_PROGRAM_INPUT, "textCoord");

	// faz a ligação entre os atributos do programa shader ao VAO e VBO ativos 
	glVertexAttribPointer(positionId, 3 /*3 elementos por vértice*/, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(normalId, 3 /*3 elementos por cores*/, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(textCoordId, 2 /*3 elementos por coordenadas da textura*/, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// ativa os atributos do programa shader ao VAO ativo
	glEnableVertexAttribArray(positionId);
	glEnableVertexAttribArray(normalId);
	glEnableVertexAttribArray(textCoordId);

	// matrizes de transformação
	Model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	View = glm::lookAt(
		cameraPosition,					// eye (posição da câmara).
		glm::vec3(0.0f, 0.0f, 0.0f),	// center (para onde está a "olhar")
		glm::vec3(0.0f, 1.0f, 0.0f)		// up
	);
	glm::mat4 ModelView = View * Model;
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));

	// obtém as localizações dos uniforms no programa shader
	GLint modelId = glGetProgramResourceLocation(programShader, GL_UNIFORM, "Model");
	GLint viewId = glGetProgramResourceLocation(programShader, GL_UNIFORM, "View");
	GLint modelViewId = glGetProgramResourceLocation(programShader, GL_UNIFORM, "ModelView");
	GLint projectionId = glGetProgramResourceLocation(programShader, GL_UNIFORM, "Projection");
	GLint normalViewId = glGetProgramResourceLocation(programShader, GL_UNIFORM, "NormalMatrix");

	// atribui o valor aos uniforms do programa shader
	glProgramUniformMatrix4fv(programShader, modelId, 1, GL_FALSE, glm::value_ptr(Model));
	glProgramUniformMatrix4fv(programShader, viewId, 1, GL_FALSE, glm::value_ptr(View));
	glProgramUniformMatrix4fv(programShader, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
	glProgramUniformMatrix4fv(programShader, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));
	glProgramUniformMatrix3fv(programShader, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

	// define a janela de renderização
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// ativa o teste de profundidade
	glEnable(GL_DEPTH_TEST);
}

void PoolBalls::display(void) {
	// limpa o buffer de cor e de profundidade
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// matrizes de transformação
	glm::mat4 ModelView = View * Model;
	NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));

	// obtém as localizações dos uniforms no programa shader
	GLint modelViewId = glGetProgramResourceLocation(programShader, GL_UNIFORM, "ModelView");
	GLint projectionId = glGetProgramResourceLocation(programShader, GL_UNIFORM, "Projection");
	GLint normalViewId = glGetProgramResourceLocation(programShader, GL_UNIFORM, "NormalMatrix");

	// atribui o valor aos uniforms do programa shader
	glProgramUniformMatrix4fv(programShader, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
	glProgramUniformMatrix4fv(programShader, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));
	glProgramUniformMatrix3fv(programShader, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

	// desenha a mesa na tela
	glBindVertexArray(tableVAO);
	glDrawArrays(GL_TRIANGLES, 0, numberOfTableVertices);

	// desenha cada bola na tela
	for (int i = 0; i < ballsVertices.size(); i++) {
		glBindVertexArray(ballsVAOs[i]);
		glDrawArrays(GL_POINTS, 0, ballsVertices[i].size() / 8);
	}
}

vector<float> PoolBalls::loadTextures(const char* filename) {
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
			vertices.push_back(textCoord.x);
			vertices.push_back(textCoord.y);
		}
	}

	return vertices;
};

#pragma endregion


#pragma region funções de callbacks para a biblioteca glfw

void PoolBalls::printErrorCallback(int code, const char* description) {
	cout << description << endl;
}

void PoolBalls::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
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

void PoolBalls::mouseCallback(GLFWwindow* window, double xpos, double ypos)
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

void PoolBalls::charCallback(GLFWwindow* window, unsigned int codepoint)
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