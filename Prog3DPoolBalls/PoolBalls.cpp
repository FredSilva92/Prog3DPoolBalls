﻿#pragma region bibliotecas

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
using namespace std;

#include <fstream>

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
const GLuint _numberOfTableVertices = 36;
GLfloat _tableVertices[_numberOfTableVertices * 8];
GLuint _tableVAO;
GLuint _tableVBO;

// bolas
const GLuint _numberOfBalls = 15;
GLuint _ballsVAOs[_numberOfBalls];
GLuint _ballsVBOs[_numberOfBalls];
vector<vector<float>> _ballsVertices;
vector<PoolBalls::Material> _ballsMaterials;

// shaders
GLuint _programShader;

// câmara
glm::mat4 _model, _view, _projection;
glm::mat3 _normalMatrix;
GLfloat _angle = 0.0f;
glm::vec3 _cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);

// eventos do mouse
float _lastX = 0.0f;
float _lastY = 0.0f;
bool _firstMouse = true;

float _zoomLevel = 1.0f;
float _minZoom = 0.1f;
float _maxZoom = 10.0f;
float _zoomSpeed = 0.1f;

#pragma endregion


#pragma region funções da biblioteca PoolBalls

void PoolBalls::init(void) {
	// posição da mesa
	float xCoord = 1.25f;
	float yCoord = 0.25f;
	float zCoord = 1.25f;

	// cores da mesa
	vector<glm::vec3> colors{
		glm::vec3(1.0f, 0.0f, 0.0f),	   // vermelho
			glm::vec3(1.0f, 1.0f, 0.0f),   // amarelo
			glm::vec3(0.0f, 1.0f, 0.0f),   // verde
			glm::vec3(0.0f, 1.0f, 1.0f),   // ciano
			glm::vec3(0.0f, 0.0f, 1.0f),   // azul
			glm::vec3(1.0f, 0.0f, 1.0f)	   // magenta
	};

	// coordenadas de textura da mesa
	float xTexture = 0.0f;
	float yTexture = 0.0f;

	// cria atributos dos vértices da mesa
	GLfloat _tableVertices[_numberOfTableVertices * 8] = {
		//*************************************************
		//                       X+ (face #0)
		// ************************************************
		// Primeiro triângulo
		// Posições						Cores											Coordenadas de textura
		xCoord, -yCoord,  zCoord,		colors[0][0], colors[0][1], colors[0][2],		xTexture, yTexture,
		xCoord, -yCoord, -zCoord,		colors[0][0], colors[0][1], colors[0][2],       xTexture, yTexture,
		xCoord,  yCoord,  zCoord,		colors[0][0], colors[0][1], colors[0][2],       xTexture, yTexture,
		// Segundo triângulo
		xCoord,  yCoord,  zCoord,		colors[0][0], colors[0][1], colors[0][2],       xTexture, yTexture,
		xCoord, -yCoord, -zCoord,		colors[0][0], colors[0][1], colors[0][2],       xTexture, yTexture,
		xCoord,  yCoord, -zCoord,		colors[0][0], colors[0][1], colors[0][2],       xTexture, yTexture,
		// ************************************************
		//                       X- (face #1)
		// ************************************************
		// Primeiro triângulo
		-xCoord, -yCoord, -zCoord,		colors[1][0], colors[1][1], colors[1][2],		xTexture, yTexture,
		-xCoord, -yCoord,  zCoord,		colors[1][0], colors[1][1], colors[1][2],		xTexture, yTexture,
		-xCoord,  yCoord, -zCoord,		colors[1][0], colors[1][1], colors[1][2],		xTexture, yTexture,
		// Segundo triângulo
		-xCoord,  yCoord, -zCoord,		colors[1][0], colors[1][1], colors[1][2],		xTexture, yTexture,
		-xCoord, -yCoord,  zCoord,		colors[1][0], colors[1][1], colors[1][2],		xTexture, yTexture,
		-xCoord,  yCoord,  zCoord,		colors[1][0], colors[1][1], colors[1][2],		xTexture, yTexture,
		// ************************************************
		//                       Y+ (face #2)
		// ************************************************
		// Primeiro triângulo
		-xCoord, yCoord, zCoord,		colors[2][0], colors[2][1], colors[2][2],      xTexture, yTexture,
		xCoord, yCoord, zCoord,			colors[2][0], colors[2][1], colors[2][2],      xTexture, yTexture,
		-xCoord, yCoord, -zCoord,		colors[2][0], colors[2][1], colors[2][2],      xTexture, yTexture,
		// Segundo triângulo
		-xCoord, yCoord, -zCoord,		colors[2][0], colors[2][1], colors[2][2],      xTexture, yTexture,
		xCoord, yCoord, zCoord,			colors[2][0], colors[2][1], colors[2][2],      xTexture, yTexture,
		xCoord, yCoord, -zCoord,		colors[2][0], colors[2][1], colors[2][2],      xTexture, yTexture,
		// ************************************************
		//                       Y- (face #3)
		// ************************************************
		// Primeiro triângulo
		-xCoord, -yCoord, -zCoord,		colors[3][0], colors[3][1], colors[3][2],		xTexture, yTexture,
		xCoord, -yCoord, -zCoord,		colors[3][0], colors[3][1], colors[3][2],		xTexture, yTexture,
		-xCoord, -yCoord, zCoord,		colors[3][0], colors[3][1], colors[3][2],		xTexture, yTexture,
		// Segundo triângulo
		-xCoord, -yCoord, zCoord,		colors[3][0], colors[3][1], colors[3][2],		xTexture, yTexture,
		xCoord, -yCoord, -zCoord,		colors[3][0], colors[3][1], colors[3][2],		xTexture, yTexture,
		xCoord, -yCoord, zCoord,		colors[3][0], colors[3][1], colors[3][2],		xTexture, yTexture,
		// ************************************************
		//                       Z+ (face #4)
		// ************************************************
		// Primeiro triângulo
		-xCoord, -yCoord, zCoord,		colors[4][0], colors[4][1], colors[4][2],      xTexture, yTexture,
		xCoord, -yCoord, zCoord,		colors[4][0], colors[4][1], colors[4][2],      xTexture, yTexture,
		-xCoord, yCoord, zCoord,		colors[4][0], colors[4][1], colors[4][2],      xTexture, yTexture,
		// Segundo triângulo
		-xCoord, yCoord, zCoord,		colors[4][0], colors[4][1], colors[4][2],      xTexture, yTexture,
		xCoord, -yCoord, zCoord,		colors[4][0], colors[4][1], colors[4][2],      xTexture, yTexture,
		xCoord,  yCoord, zCoord,		colors[4][0], colors[4][1], colors[4][2],      xTexture, yTexture,
		// ************************************************
		//                       Z- (face #5)
		// ************************************************
		// Primeiro triângulo
		xCoord, -yCoord, -zCoord,		colors[5][0], colors[5][1], colors[5][2],      xTexture, yTexture,
		-xCoord, -yCoord, -zCoord,		colors[5][0], colors[5][1], colors[5][2],      xTexture, yTexture,
		xCoord, yCoord, -zCoord,		colors[5][0], colors[5][1], colors[5][2],      xTexture, yTexture,
		// Segundo triângulo
		xCoord, yCoord, -zCoord,		colors[5][0], colors[5][1], colors[5][2],      xTexture, yTexture,
		-xCoord, -yCoord, -zCoord,		colors[5][0], colors[5][1], colors[5][2],      xTexture, yTexture,
		-xCoord, yCoord, -zCoord,		colors[5][0], colors[5][1], colors[5][2],      xTexture, yTexture
	};

	// gera o nome para o VAO da mesa
	glGenVertexArrays(1, &_tableVAO);

	// vincula o VAO ao contexto OpenGL atual
	glBindVertexArray(_tableVAO);

	// gera o nome para o VBO da mesa
	glGenBuffers(1, &_tableVBO);

	// vincula o VBO ao contexto OpenGL atual
	glBindBuffer(GL_ARRAY_BUFFER, _tableVBO);

	// inicializa o VBO atualmente ativo com dados imutáveis
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(_tableVertices), _tableVertices, 0);

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

	// carrega o modelo e material de cada bola
	for (int i = 1; i <= _numberOfBalls; i++) {
		std::string objFilename = "textures/Ball" + to_string(i) + ".obj";
		_ballsVertices.push_back(PoolBalls::load3dModel(objFilename.c_str()));

		std::string mtlFilename = getMtlFromObj(objFilename.c_str());
		_ballsMaterials.push_back(PoolBalls::loadMaterial(mtlFilename.c_str()));
	}

	// guarda os nomes dos ficheiros das texturas
	vector<string> textureFilenames;
	for (int i = 0; i < 2/*_numberOfBalls*/; i++) {
		textureFilenames.push_back(_ballsMaterials[i].mapKd);
	}

	// carrega a textura de cada bola
	for (int i = 0; i < 2/*_numberOfBalls*/; i++) {
		PoolBalls::loadTextures(textureFilenames);
	}

	// gera nomes para os VAOs das bolas
	glGenVertexArrays(_numberOfBalls, _ballsVAOs);

	// vincula cada VAO das bolas ao contexto OpenGL atual
	for (int i = 0; i < _numberOfBalls; i++) {
		glBindVertexArray(_ballsVAOs[i]);
	}

	// gera nomes para os VBOs das bolas
	glGenBuffers(_numberOfBalls, _ballsVBOs);

	// cria e configurar cada VBO das bolas
	for (int i = 0; i < _ballsVertices.size(); i++) {
		// vincula o VBO ao contexto OpenGL atual
		glBindBuffer(GL_ARRAY_BUFFER, _ballsVBOs[i]);

		// inicializa o vbo atualmente ativo com dados imutáveis
		glBufferStorage(GL_ARRAY_BUFFER, _ballsVertices[i].size() * sizeof(float), _ballsVertices[i].data(), 0);

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
	_programShader = loadShaders(shaders);

	// se houve erros ao carregar shaders
	if (!_programShader) {
		cout << "Erro ao carregar shaders: " << endl;
		exit(EXIT_FAILURE);
	}

	// vincula o programa shader ao contexto OpenGL atual
	glUseProgram(_programShader);

	// obtém as localizações dos atributos no programa shader
	GLint positionId = glGetProgramResourceLocation(_programShader, GL_PROGRAM_INPUT, "vPosition");
	GLint normalId = glGetProgramResourceLocation(_programShader, GL_PROGRAM_INPUT, "vNormal");
	GLint textCoordId = glGetProgramResourceLocation(_programShader, GL_PROGRAM_INPUT, "vTextureCoords");

	// faz a ligação entre os atributos do programa shader ao VAO e VBO ativos 
	glVertexAttribPointer(positionId, 3 /*3 elementos por vértice*/, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(normalId, 3 /*3 elementos por cor*/, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(textCoordId, 2 /*3 elementos por coordenadas da textura*/, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// ativa os atributos do programa shader ao VAO ativo
	glEnableVertexAttribArray(positionId);
	glEnableVertexAttribArray(normalId);
	glEnableVertexAttribArray(textCoordId);

	// matrizes de transformação
	_model = glm::rotate(glm::mat4(1.0f), _angle, glm::vec3(0.0f, 1.0f, 0.0f));
	_view = glm::lookAt(
		_cameraPosition,					// eye (posição da câmara).
		glm::vec3(0.0f, 0.0f, 0.0f),	// center (para onde está a "olhar")
		glm::vec3(0.0f, 1.0f, 0.0f)		// up
	);
	glm::mat4 modelView = _view * _model;
	_projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	_normalMatrix = glm::inverseTranspose(glm::mat3(modelView));

	// obtém as localizações dos uniforms no programa shader
	GLint modelId = glGetProgramResourceLocation(_programShader, GL_UNIFORM, "Model");
	GLint viewId = glGetProgramResourceLocation(_programShader, GL_UNIFORM, "View");
	GLint modelViewId = glGetProgramResourceLocation(_programShader, GL_UNIFORM, "ModelView");
	GLint projectionId = glGetProgramResourceLocation(_programShader, GL_UNIFORM, "Projection");
	GLint normalViewId = glGetProgramResourceLocation(_programShader, GL_UNIFORM, "NormalMatrix");

	// atribui o valor aos uniforms do programa shader
	glProgramUniformMatrix4fv(_programShader, modelId, 1, GL_FALSE, glm::value_ptr(_model));
	glProgramUniformMatrix4fv(_programShader, viewId, 1, GL_FALSE, glm::value_ptr(_view));
	glProgramUniformMatrix4fv(_programShader, modelViewId, 1, GL_FALSE, glm::value_ptr(modelView));
	glProgramUniformMatrix4fv(_programShader, projectionId, 1, GL_FALSE, glm::value_ptr(_projection));
	glProgramUniformMatrix3fv(_programShader, normalViewId, 1, GL_FALSE, glm::value_ptr(_normalMatrix));

	// define a janela de renderização
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// ativa o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);



	/**************************************/
	/*   PODE SER ÚTIL PARA AS TEXTURAS   */
	/**************************************/

	////glUniform1i(glGetUniformLocation(programa, "TextureSampler1"), 0 /* Unidade de Textura #0 */);
	////glUniform1i(glGetUniformLocation(programa, "TextureSampler2"), 1 /* Unidade de Textura #1 */);

	//// indicação da unidade de textura a ligar ao sampler 'TexSampler1'.
	//GLint locationTextureSampler1 = glGetProgramResourceLocation(_programShader, GL_UNIFORM, "TexSampler1");
	//glProgramUniform1i(_programShader, locationTextureSampler1, 0 /* Unidade de Textura #0 */);

	//// indicação da unidade de textura a ligar ao sampler 'TexSampler2'.
	//GLint locationTextureSampler2 = glGetProgramResourceLocation(_programShader, GL_UNIFORM, "TexSampler2");
	//glProgramUniform1i(_programShader, locationTextureSampler2, 1 /* Unidade de Textura #1 */);
}

void PoolBalls::display(void) {
	// limpa o buffer de cor e de profundidade
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// translação da mesa
	glm::mat4 translatedModel = glm::translate(_model, glm::vec3(0.0f, 0.0f, 0.0f));

	// modelo de visualização do objeto
	glm::mat4 modelView = _view * translatedModel;

	// obtém a localização do uniform
	GLint modelViewId = glGetProgramResourceLocation(_programShader, GL_UNIFORM, "ModelView");

	// atribui o valor ao uniform
	glProgramUniformMatrix4fv(_programShader, modelViewId, 1, GL_FALSE, glm::value_ptr(modelView));

	// desenha a mesa na tela
	glBindVertexArray(_tableVAO);
	glDrawArrays(GL_TRIANGLES, 0, _numberOfTableVertices);

	// para cada bola
	for (int i = 0; i < _ballsVertices.size(); i++) {
		// translação da mesa
		translatedModel = glm::translate(_model, glm::vec3(0.0f, 0.0f, 0.0f));

		// testar outra posição de uma bola
		if (i == 0) {
			// translação da mesa
			translatedModel = glm::translate(_model, glm::vec3(1.0f, 0.0f, 0.0f));
		}

		// modelo de visualização do objeto
		modelView = _view * translatedModel;

		// obtém a localização do uniform
		modelViewId = glGetProgramResourceLocation(_programShader, GL_UNIFORM, "ModelView");

		// atribui o valor ao uniform
		glProgramUniformMatrix4fv(_programShader, modelViewId, 1, GL_FALSE, glm::value_ptr(modelView));

		// desenha a bola na tela
		glBindVertexArray(0);
		glDrawArrays(GL_POINTS, 0, _ballsVertices[i].size() / 8);
	}
}

std::vector<float> PoolBalls::load3dModel(const char* objFilename) {
	std::vector<float> vertices;

	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warning, error;

	// se houve erros ao carregar o ficheiro .obj
	if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, objFilename)) {
		std::cout << warning << error << '\n';
		return {};
	}

	// lê atributos do modelo 3D
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			glm::vec3 position = {
				attributes.vertices[index.vertex_index],
				attributes.vertices[index.vertex_index + 1],
				attributes.vertices[index.vertex_index + 2]
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

			vertices.push_back(position.x);
			vertices.push_back(position.y);
			vertices.push_back(position.z);
			vertices.push_back(normal.x);
			vertices.push_back(normal.y);
			vertices.push_back(normal.z);
			vertices.push_back(textCoord.x);
			vertices.push_back(textCoord.y);
		}
	}

	return vertices;
}

std::string PoolBalls::getMtlFromObj(const char* objFilename) {
	std::ifstream file(objFilename);
	std::string line;
	std::string mtlFilename;

	while (std::getline(file, line)) {
		std::istringstream stream(line);
		std::string prefix;
		stream >> prefix;

		if (prefix == "mtllib") {
			stream >> mtlFilename;
			break;
		}
	}

	return mtlFilename;
}

PoolBalls::Material PoolBalls::loadMaterial(const char* mtlFilename) {
	std::string directory = "textures/";
	std::ifstream mtlFile(directory + mtlFilename);

	// se houve erros ao carregar o ficheiro .mtl
	if (!mtlFile) {
		std::cerr << "Erro ao carregar ficheiro .mtl." << std::endl;
		return {};
	}

	std::string line;
	std::string materialName;
	PoolBalls::Material material;

	// lê cada linha do ficheiro .mtl
	while (std::getline(mtlFile, line)) {
		// ignora linhas vazias ou comentadas
		if (line.empty() || line[0] == '#') {
			continue;
		}

		std::istringstream stream(line);
		std::string command;
		stream >> command;

		// nome (cria um novo material)
		if (command == "newmtl") {
			std::string name;
			stream >> name;

			material.newmtl = name;
		}
		// coeficiente de especularidade
		else if (command == "Ns") {
			float value;
			stream >> value;

			material.ns = value;
		}
		// cor ambiente
		else if (command == "Ka") {
			float r, g, b;
			stream >> r >> g >> b;

			material.ka = { r, g, b };
		}
		// cor difusa
		else if (command == "Kd") {
			float r, g, b;
			stream >> r >> g >> b;

			material.kd = { r, g, b };
		}
		// cor especular
		else if (command == "Ks") {
			float r, g, b;
			stream >> r >> g >> b;

			material.ks = { r, g, b };
		}
		// índice de refração
		else if (command == "Ni") {
			float value;
			stream >> value;

			material.ni = value;
		}
		// índice de refração
		else if (command == "d") {
			float value;
			stream >> value;

			material.d = value;
		}
		// modelo de iluminação
		else if (command == "illum") {
			float value;
			stream >> value;

			material.illum = value;
		}
		// textura
		else if (command == "map_Kd") {
			std::string texture;
			stream >> texture;

			material.mapKd = texture;
		}
	}

	return material;
}

void PoolBalls::loadTextures(std::vector<string> imageFilenames) {
	GLuint textureName[2/*_numberOfBalls*/]{};
	GLint texCount = 0;

	// gera nomes para as texturas
	glGenTextures(2/*_numberOfBalls*/, textureName);

	// lê cada imagem das texturas
	for (auto filename : imageFilenames) {
		// ativa a Unidade de Textura #(0 + texCount),
		// a Unidade de Textura 0 está ativa por defeito,
		// só uma Unidade de Textura pode estar ativa
		glActiveTexture(GL_TEXTURE0 + texCount);

		// vincula um nome de textura ao target GL_TEXTURE_2D da unidade de rextura ativa.
		glBindTexture(GL_TEXTURE_2D, textureName[texCount++]);

		// define os parâmetros de filtragem (wrapping e ajuste de tamanho),
		// para a textura que está vinculada ao target GL_TEXTURE_2D da unidade de rextura ativa.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// dados da imagem
		int width, height, nChannels;

		// ativa a inversão vertical da imagem, aquando da sua leitura para memória
		stbi_set_flip_vertically_on_load(true);

		std::string directory = "textures/";
		std::string fullPath = directory + filename;
		unsigned char* imageData = stbi_load(fullPath.c_str(), &width, &height, &nChannels, 0);

		// se houve erros ao abrir o ficheiro
		if (!imageData) {
			cout << "Error loading texture!" << endl;
			return;
		}

		// carrega os dados da imagem para o Objeto de Textura vinculado ao target GL_TEXTURE_2D da unidade de textura ativa.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

		// gera o mipmap para essa textura
		glGenerateMipmap(GL_TEXTURE_2D);

		// liberta a imagem da memória do CPU
		stbi_image_free(imageData);
	}
}

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
	float zoomFactor = 1.0f + static_cast<float>(yoffset) * _zoomSpeed;

	_view = glm::scale(_view, glm::vec3(zoomFactor, zoomFactor, 1.0f));

	_zoomLevel += yoffset * _zoomSpeed;
	_zoomLevel = std::max(_minZoom, std::min(_maxZoom, _zoomLevel));
}

void PoolBalls::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
	{
		_lastX = xpos;
		_lastY = ypos;
		_firstMouse = false;
		return; // Sai da função se o botão do mouse esquerdo não estiver pressionado
	}

	if (_firstMouse)
	{
		_lastX = xpos;
		_lastY = ypos;
		_firstMouse = false;
	}

	float xOffset = xpos - _lastX;
	float yOffset = ypos - _lastY;

	_lastX = xpos;
	_lastY = ypos;

	float sensitivity = 0.1f;

	// Aplicar rotação horizontal 
	_view = glm::rotate(_view, glm::radians(xOffset), glm::vec3(0.0f, 1.0f, 0.0f));

	// Calcular o vetor direito da câmera
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(_view[2])));

	// Aplicar rotação vertical 
	_view = glm::rotate(_view, glm::radians(yOffset), right);
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