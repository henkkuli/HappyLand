#pragma once

class player;
class playerManager;

#include <math/vec.h>


class game;
class camera;
class player {
public:
	player();
	~player();

protected:
	math::vec3 position;
	float horizontalRotation, verticalRotation;
	float speed;

	friend class playerManager;
};

class playerManager {
public:
	playerManager(const game&, camera&);
	
	void update(double delta);
private:
	player player;
	const game &game;
	camera &camera;
};