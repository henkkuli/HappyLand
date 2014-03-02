#pragma once

class bone;
class model;
class modelLoader;

#include <buffers.h>
#include <texture.h>
#include <string>
#include <math/vec.h>

class bone {
public:
	unsigned short parentIndex;
	math::vec3 head, tail;
	string name;
};

class model {
public:
	model() {
		me = new _modelData;
		me->instanceCounter = 1;
	}

	model(const model &other) {
		me = other.me;
		me->instanceCounter++;
	}

	model& operator=(const model &other) {
		me->instanceCounter--;
		if (me->instanceCounter <= 0)
			delete me;

		me = other.me;
		me->instanceCounter++;
		return *this;
	}

	~model() {
		me->instanceCounter--;
		if (me->instanceCounter <= 0)
			delete me;
	}

	void render() {
		me->vao.bind();
		me->indexBuffer.bind(elementArrayBuffer);
		me->diffuse.bind(0);
		me->specular.bind(1);
		glDrawElements(
			GL_TRIANGLES,
			me->indexCount * 3,
			GL_UNSIGNED_SHORT,
			(void*)0
			);
	}

	unsigned short getBoneCount() const {
		return me->boneCount;
	}

	bone* getBones() {
		return me->bones;
	}

private:
	struct _modelData {
		_modelData() : vertexBuffer(arrayBuffer), indexBuffer(elementArrayBuffer), diffuse(texture2DType), specular(texture2DType), bones(0) {
		}
		~_modelData() {
			if (bones)
				delete[] bones;
		}
		buffer vertexBuffer, indexBuffer;
		texture diffuse, specular;
		vao vao;
		unsigned short indexCount;

		unsigned short boneCount;
		bone *bones;

		unsigned int instanceCounter;
	} *me;

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
