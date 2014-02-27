#pragma once

class model;
class modelLoader;

#include <buffers.h>
#include <texture.h>
#include <string>

class model {
public:
	model();
	~model();

	void render() {
		vao.bind();
		indexBuffer.bind(elementArrayBuffer);
		diffuse.bind(0);
		specular.bind(1);
		glDrawElements(
			GL_TRIANGLES,
			indexCount * 3,
			GL_UNSIGNED_SHORT,
			(void*) 0
		);
	}

	unsigned short getBoneCount() const {
		return boneCount;
	}

private:
	buffer vertexBuffer, indexBuffer;
	texture diffuse, specular;
	vao vao;
	unsigned short indexCount;

	unsigned short boneCount;

	friend class modelLoader;
};

class modelLoader {
public:
	modelLoader(std::string _basePath) : basePath(_basePath) {
	}
public:
	model load(const std::string filename, const std::string diffuceFilename = "", const std::string specularFilename = "");
	const std::string basePath;
};
