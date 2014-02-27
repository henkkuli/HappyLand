#include "model.h"

#include <fstream>
#include <math/vec.h>

using namespace std;
using namespace math;

model::model() : vertexBuffer(arrayBuffer), indexBuffer(elementArrayBuffer), diffuse(texture2DType), specular(texture2DType) {
}


model::~model() {
}

#pragma pack(push, 1)
struct vertex {
	vec3 position;
	vec2 uv;
	vec3 normal;
	unsigned char index[2];
	float weight[2];
};
#pragma pack(pop)

model modelLoader::load(const std::string filename, const std::string diffuceFilename, const std::string specularFilename) {
	ifstream file(basePath + '/' + filename, ios::in | ios::binary);
	model model;
	unsigned short vertexCount, indexCount;

	// Read the header
	file.read((char*) &vertexCount, 2);
	file.read((char*) &indexCount, 2);

	// Read the vertex data
	vertex *vertices = new vertex[vertexCount];
	unsigned short *indices = new unsigned short[indexCount*3];

	file.read((char*) vertices, sizeof(vertex) * vertexCount);
	file.read((char*) indices, sizeof(unsigned short) * 3 * indexCount);

	// Set tp buffers
	model.vertexBuffer.allocate(vertices, vertexCount);
	model.indexBuffer.allocate(indices, indexCount * 3);
	model.indexCount = indexCount;

	model.vao.setAttribute(0, 3, model.vertexBuffer, offsetof(vertex, position));
	model.vao.setAttribute(1, 2, model.vertexBuffer, offsetof(vertex, uv));
	model.vao.setAttribute(2, 3, model.vertexBuffer, offsetof(vertex, normal));
	model.vao.setAttribute(3, 2, GL_UNSIGNED_BYTE, false, model.vertexBuffer, offsetof(vertex, index));
	model.vao.setAttribute(4, 2, model.vertexBuffer, offsetof(vertex, weight));
	model.vao.setBuffer(model.indexBuffer, elementArrayBuffer);

	// Read bones
	// TODO!!!
	model.boneCount = 4;		// Just an emergency solution

	// Free some memory
	delete[] vertices;
	delete[] indices;

	// Textures
	textureLoader tloader(basePath + "/textures/");
	if (diffuceFilename != "") {
		tloader.loadToTexture(model.diffuse, diffuceFilename, texture2DType, pngFile);
		model.diffuse.setParameter(parameterMagFilter, GL_LINEAR);
		model.diffuse.setParameter(parameterMinFilter, GL_LINEAR);
	}
	if (specularFilename != "") {
		tloader.loadToTexture(model.specular, specularFilename, texture2DType, pngFile);
		model.specular.setParameter(parameterMagFilter, GL_LINEAR);
		model.specular.setParameter(parameterMinFilter, GL_LINEAR);
	}

	return model;
}