#pragma once

class game;

#include "manager.h"
#include "renderManager.h"
#include "entity.h"
#include "camera.h"
#include "player.h"
#include "viewport.h"

class game {
public:
	game(viewport &);
	~game();

	void init();
	void run();
	void stop();

	bool isKeyPressed(int key) const {
		return viewport.isKeyPressed(key);
	}
	math::vec2 getMouseDelta() const {
		return viewport.getMouseDelta();
	}
	math::vec2 getMouseScrollDelta() const {
		return viewport.getMouseScrollDelta();
	}

private:
	// Prevent copying
	game(const game&);
	game& operator=(const game&);

	viewport &viewport;
	camera camera;

	entityManager entityManager;
	entityRenderManager entityRenderManager;
	playerManager playerManager;
};

