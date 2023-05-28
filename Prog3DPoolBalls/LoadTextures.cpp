#pragma once

#include <iostream>
#include <fstream>

#define GLEW_STATIC
#include <GL\glew.h>

#include "LoadShaders.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "thirdParty/tiny_obj_loader.h"

#include <glm\glm.hpp>

#include <vector>

using namespace std;

vector<float> LoadTextures(const char* filename) {
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
				attributes.normals[ index.normal_index + 2]
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