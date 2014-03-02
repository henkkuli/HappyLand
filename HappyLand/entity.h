#pragma once

class entity;

#include <math/vec.h>
#include <math/mat.h>
#include <math/versor.h>
#include <buffers.h>

class game;
class entityRenderManager;

class entity {
public:
	entity(const game&, const math::vec3&, const math::vec3&);
	virtual ~entity();

	virtual void update(double delta) = 0;
	virtual math::mat4 getTranslation() {
		return math::mat4::translation(position);
	}

protected:
	const game &game;
	math::vec3 position, velocity;
};

#define ENTITY_MAX_BONES 4
#include "model.h"
class modelEntity : public entity {
public:
	modelEntity(const ::game &game, const model &_model) : entity(game, math::vec3(), math::vec3()), model(_model) {
	}
	modelEntity(const ::game &game, const model &_model, const math::vec3 &_position) : entity(game, _position, math::vec3()), model(_model) {
	}

	virtual void update(double delta);

private:
	void updateBoneMatrices();

	model model;
	math::mat4 boneMatrices[ENTITY_MAX_BONES];
	math::mat4 nodeMatrices[ENTITY_MAX_BONES];
	struct boneStatus {
		math::vec3 scale;
		math::versor versor;
	} boneStatuses[ENTITY_MAX_BONES];

	friend class entityRenderManager;
};