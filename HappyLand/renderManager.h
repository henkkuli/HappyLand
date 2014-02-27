#pragma once

template<class T>
class renderManager;
template<class T>
class cameraRenderManager;
class entityRenderManager;

#include "manager.h"
#include "camera.h"

template<class T>
class renderManager {
public:
	virtual ~renderManager() {}

	virtual void render() = 0;

protected:
	renderManager(manager<T> &_manager) : manager(_manager) {
	}

	typedef typename manager<T>::iterator iterator;

	// Copy protected members of manager
	iterator begin() {
		return manager.begin();
	}
	iterator end() {
		return manager.end();
	}

private:
	// Prevent copying
	renderManager(const renderManager&);
	renderManager& operator=(const renderManager&);

	manager<T> &manager;
};

template<class T>
class cameraRenderManager : public renderManager<T> {
public:
protected:
	cameraRenderManager(::manager<T> &_manager, camera &_camera) : renderManager(_manager), camera(_camera) {
	}
	
	math::mat4 const &getProjection() const {
		return camera.getProjection();
	}
	math::mat4 const &getView() const {
		return camera.getView();
	}

private:
	camera &camera;
};

// Specific render managers

#include "entity.h"
#include <shaders.h>
#include <buffers.h>
class entityRenderManager : public cameraRenderManager<entity> {
public:
	entityRenderManager(::manager<entity> &_manager, ::camera &_camera) : cameraRenderManager(_manager, _camera) {
		prog.attachShader(shader("shaders/entity_vs.glsl", vertexShader));
		prog.attachShader(shader("shaders/entity_fs.glsl", fragmentShader));
		prog.link();
		
		projectionUniform = prog.getUniform("proj");
		viewUniform = prog.getUniform("view");
		modelUniform = prog.getUniform("model");
		diffuseMapUniform = prog.getUniform("diffuseMap");
		specularMapUniform = prog.getUniform("specularMap");
		boneUniform = prog.getUniform("bones");
	}

	virtual ~entityRenderManager() {
	}

	virtual void render();

private:
	program prog;
	uniform projectionUniform, viewUniform, modelUniform, diffuseMapUniform, specularMapUniform, boneUniform;
};
