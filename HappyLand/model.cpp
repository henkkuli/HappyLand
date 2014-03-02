#include "model.h"

#include <fstream>
#include <math/vec.h>
#include <math/versor.h>

using namespace std;
using namespace math;

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
	unsigned short vertexCount, indexCount, boneCount;

	// Read the header
	file.read((char*)&vertexCount, 2);
	file.read((char*)&indexCount, 2);
	file.read((char*)&boneCount, 2);

	// Read the vertex data
	vertex *vertices = new vertex[vertexCount];
	unsigned short *indices = new unsigned short[indexCount*3];

	file.read((char*) vertices, sizeof(vertex) * vertexCount);
	file.read((char*) indices, sizeof(unsigned short) * 3 * indexCount);

	// Read bones
	model.me->boneCount = boneCount;
	model.me->bones = new bone[boneCount];
	for (unsigned short i = 0; i < boneCount; i++) {
		unsigned short nameLength;
		bone bone;

		// Read the bone from the file
		file.read((char*)&nameLength, 2);
		copy_n(istream_iterator<char>(file), nameLength, insert_iterator<string>(bone.name, bone.name.begin()));
		file.read((char*)&bone.parentIndex, 2);
		file.read((char*)&bone.head, sizeof(vec3));
		file.read((char*)&bone.tail, sizeof(vec3));

		// Uncomment to see bone names and their head and tail location
		// cout << bone.name << "(" << bone.head.x() << "," << bone.head.y() << "," << bone.head.z() << ")" << "(" << bone.tail.x() << "," << bone.tail.y() << "," << bone.tail.z() << ")" << endl;

		// Copy bone to the model
		model.me->bones[i] = bone;
	}

	// Uncomment to see the groups vertices are attached to and their weights
	/*
	for (int i = 0; i < vertexCount; i++) {
		vertex &v = vertices[i];
		cout << v.index[0] + 0 << "@" << v.weight[0] << "&" << v.index[1] + 0 << "@" << v.weight[1] << " ";
	}
	cout << endl;
	*/

	// Set tp buffers
	model.me->vertexBuffer.allocate(vertices, vertexCount);
	model.me->indexBuffer.allocate(indices, indexCount * 3);
	model.me->indexCount = indexCount;

	model.me->vao.setAttribute(0, 3, model.me->vertexBuffer, offsetof(vertex, position));
	model.me->vao.setAttribute(1, 2, model.me->vertexBuffer, offsetof(vertex, uv));
	model.me->vao.setAttribute(2, 3, model.me->vertexBuffer, offsetof(vertex, normal));
	model.me->vao.setAttribute(3, 2, GL_UNSIGNED_BYTE, false, model.me->vertexBuffer, offsetof(vertex, index));
	model.me->vao.setAttribute(4, 2, model.me->vertexBuffer, offsetof(vertex, weight));
	model.me->vao.setBuffer(model.me->indexBuffer, elementArrayBuffer);

	// Free some memory
	delete[] vertices;
	delete[] indices;

	// Textures
	textureLoader tloader(basePath + "/textures/");
	if (diffuceFilename != "") {
		tloader.loadToTexture(model.me->diffuse, diffuceFilename, texture2DType, pngFile);
		model.me->diffuse.setParameter(parameterMagFilter, GL_LINEAR);
		model.me->diffuse.setParameter(parameterMinFilter, GL_LINEAR);
	}
	if (specularFilename != "") {
		tloader.loadToTexture(model.me->specular, specularFilename, texture2DType, pngFile);
		model.me->specular.setParameter(parameterMagFilter, GL_LINEAR);
		model.me->specular.setParameter(parameterMinFilter, GL_LINEAR);
	}

	return model;
}