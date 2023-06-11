﻿#pragma region importações

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <vector>
#include <string>
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
#include "thirdParty/TinyObjLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "thirdParty/StbImage.h"

#include "LoadShaders.h"
#include "PoolBalls.h"
#include "Source.h"

#pragma endregion


namespace PoolBalls {

#pragma region funções principais da biblioteca PoolBalls

	void RendererBall::Read(const std::string obj_model_filepath) {
		_objFilepath = obj_model_filepath.c_str();

		std::vector<float> vertices;

		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warning, error;

		// se houve erros ao carregar o ficheiro .obj
		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, _objFilepath)) {
			std::cout << warning << error << '\n';
			return;
		}

		// lê atributos do modelo 3D
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				glm::vec4 pos = {
					attributes.vertices[3 * index.vertex_index],
					attributes.vertices[3 * index.vertex_index + 1],
					attributes.vertices[3 * index.vertex_index + 2],
					1
				};

				glm::vec3 normal = {
					attributes.normals[3 * index.normal_index],
					attributes.normals[3 * index.normal_index + 1],
					attributes.normals[3 * index.normal_index + 2]
				};

				glm::vec2 textCoord = {
					attributes.texcoords[2 * index.texcoord_index],
					attributes.texcoords[2 * index.texcoord_index + 1]
				};

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

		// armazena o modelo 3D
		_ballVertices = load3dModel(_objFilepath);

		// armazena o material
		std::string mtlFilename = getMtlFromObj(_objFilepath);
		_ballMaterial = loadMaterial(mtlFilename.c_str());

		// armazena a textura
		std::string textureFilename = _ballMaterial.map_kd;
		_ballTexture = loadTexture(textureFilename);
	}

	void RendererBall::Send(void) {
		// gera o nome para o VAO da bola
		glGenVertexArrays(1, _ballVAO);

		// vincula o VAO da bola ao contexto OpenGL atual
		glBindVertexArray(*_ballVAO);

		// gera o nome para o VBO da bola
		glGenBuffers(1, _ballVBO);

		// para cada vértice da da bola
		for (int i = 0; i < _ballVertices.size(); i++) {
			// vincula o VBO ao contexto OpenGL atual
			glBindBuffer(GL_ARRAY_BUFFER, *_ballVBO);

			// inicializa o VBO atualmente ativo com dados imutáveis
			glBufferStorage(GL_ARRAY_BUFFER, _ballVertices.size() * sizeof(float), _ballVertices.data(), 0);

			// ativa atributos das posições dos vértices
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(0);

			// ativa atributos das cores dos vértices
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			// ativa atributos das coordenadas de textura dos vértices
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);

			// desvincula o VAO atual
			glBindVertexArray(0);
		}

		// gera nomes para as texturas
		GLuint textureName;
		glGenTextures(1, &textureName);

		// saber atual unidade de textura para ativar a próxima
		GLint currentTextureUnits;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureUnits);

		// ativa a unidade de textura atual (inicia na unidade 0)
		glActiveTexture(GL_TEXTURE0 + currentTextureUnits);

		// vincula um nome de textura ao target GL_TEXTURE_2D da unidade de textura ativa
		glBindTexture(GL_TEXTURE_2D, textureName);

		// define os parâmetros de filtragem (wrapping e ajuste de tamanho)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// carrega os dados da imagem para o objeto de textura vinculado ao target GL_TEXTURE_2D da unidade de textura ativa
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _ballTexture.width, _ballTexture.height, 0, _ballTexture.nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, _ballTexture.image);

		// gera o mipmap para essa textura
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void RendererBall::Draw(glm::vec3 position, glm::vec3 orientation) {

	}

#pragma endregion


#pragma region funções secundárias da biblioteca PoolBalls

	std::vector<float> RendererBall::load3dModel(const char* objFilepath) {
		std::vector<float> vertices;

		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warning, error;

		// se houve erros ao carregar o ficheiro .obj
		if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, objFilepath)) {
			std::cout << warning << error << '\n';
			return {};
		}

		// lê atributos do modelo 3D
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				glm::vec4 pos = {
					attributes.vertices[3 * index.vertex_index],
					attributes.vertices[3 * index.vertex_index + 1],
					attributes.vertices[3 * index.vertex_index + 2],
					1
				};

				glm::vec3 normal = {
					attributes.normals[3 * index.normal_index],
					attributes.normals[3 * index.normal_index + 1],
					attributes.normals[3 * index.normal_index + 2]
				};

				glm::vec2 textCoord = {
					attributes.texcoords[2 * index.texcoord_index],
					attributes.texcoords[2 * index.texcoord_index + 1]
				};

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
	}

	std::string RendererBall::getMtlFromObj(const char* objFilepath) {
		std::ifstream file(objFilepath);
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

	Material RendererBall::loadMaterial(const char* mtlFilename) {
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

		// lê cada linha do ficheiro .mtl e guarda os respetivos dados
		while (std::getline(mtlFile, line)) {
			// ignora linhas vazias ou comentadas
			if (line.empty() || line[0] == '#') {
				continue;
			}

			std::istringstream stream(line);
			std::string command;
			stream >> command;

			// brilho
			if (command == "Ns") {
				float value;
				stream >> value;

				material.ns = value;
			}
			// cor ambiente
			else if (command == "Ka") {
				float r, g, b;
				stream >> r >> g >> b;

				material.ka = glm::vec3(r, g, b);
			}
			// cor difusa
			else if (command == "Kd") {
				float r, g, b;
				stream >> r >> g >> b;

				material.kd = glm::vec3(r, g, b);
			}
			// cor especular
			else if (command == "Ks") {
				float r, g, b;
				stream >> r >> g >> b;

				material.ks = glm::vec3(r, g, b);
			}
			// textura
			else if (command == "map_Kd") {
				std::string texture;
				stream >> texture;

				material.map_kd = texture;
			}
		}

		return material;
	}

	Texture RendererBall::loadTexture(std::string imageFilename) {
		Texture texture{};
		int width, height, nChannels;

		// lê o ficheiro da imagem
		std::string directory = "textures/";
		std::string fullPath = directory + imageFilename;
		unsigned char* image = stbi_load(fullPath.c_str(), &width, &height, &nChannels, 0);

		// se houve erros ao abrir o ficheiro
		if (!image) {
			std::cerr << "Erro ao abrir o ficheiro '" << fullPath.c_str() << "'." << std::endl;
			return {};
		}

		// aloca memória para a imagem
		unsigned char* imageCopy = new unsigned char[width * height * nChannels];

		// copia a imagem para a nova área de memória
		std::memcpy(imageCopy, image, width * height * nChannels);

		texture.width = width;
		texture.height = height;
		texture.nChannels = nChannels;
		texture.image = imageCopy;

		// liberta a imagem da memória do CPU
		stbi_image_free(image);

		return texture;
	}

	void RendererBall::LoadMaterialLighting(GLuint programShader, Material material) {
		glProgramUniform1f(programShader, glGetProgramResourceLocation(programShader, GL_UNIFORM, "material.shininess"), material.ns);
		glProgramUniform3fv(programShader, glGetProgramResourceLocation(programShader, GL_UNIFORM, "material.ambient"), 1, glm::value_ptr(material.ka));
		glProgramUniform3fv(programShader, glGetProgramResourceLocation(programShader, GL_UNIFORM, "material.diffuse"), 1, glm::value_ptr(material.kd));
		glProgramUniform3fv(programShader, glGetProgramResourceLocation(programShader, GL_UNIFORM, "material.specular"), 1, glm::value_ptr(material.ks));
	}

#pragma endregion

}