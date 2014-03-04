#include "player.h"

#include "game.h"
#include "camera.h"

using namespace math;

#define PLAYER_HEAD_MAX 1.5f
#define PLAYER_HEAD_MIN -1.5f

player::player() : horizontalRotation(0), verticalRotation(0), speed(1) {
}


player::~player() {
}

playerManager::playerManager(const ::game &_game, ::camera &_camera) : game(_game), camera(_camera) {
}

void playerManager::update(double delta) {
	float deltaf = (float) delta;

	// Keyboard
	if (game.isKeyPressed(GLFW_KEY_W)) {
		player.position += math::vec3(sinf(player.horizontalRotation) * deltaf * player.speed, 0, cosf(player.horizontalRotation) * deltaf * player.speed);
	}
	if (game.isKeyPressed(GLFW_KEY_S)) {
		player.position -= math::vec3(sinf(player.horizontalRotation) * deltaf * player.speed, 0, cosf(player.horizontalRotation) * deltaf * player.speed);
	}
	if (game.isKeyPressed(GLFW_KEY_D)) {
		player.position -= math::vec3(cosf(player.horizontalRotation) * deltaf * player.speed, 0, -sinf(player.horizontalRotation) * deltaf * player.speed);
	}
	if (game.isKeyPressed(GLFW_KEY_A)) {
		player.position += math::vec3(cosf(player.horizontalRotation) * deltaf * player.speed, 0, -sinf(player.horizontalRotation) * deltaf * player.speed);
	}
	if (game.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		player.position.y += deltaf * player.speed;
	}
	if (game.isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
		player.position.y -= deltaf * player.speed;
	}

	// Mouse
	vec2 mouseDelta = game.getMouseDelta() * 0.001f;
	player.horizontalRotation -= mouseDelta.x;
	player.verticalRotation -= mouseDelta.y;
	player.speed += game.getMouseScrollDelta().y;

	// Restrict player head position
	if (player.verticalRotation < PLAYER_HEAD_MIN) player.verticalRotation = PLAYER_HEAD_MIN;
	if (player.verticalRotation > PLAYER_HEAD_MAX) player.verticalRotation = PLAYER_HEAD_MAX;
	if (player.speed < 0) player.speed = 0;

	// Copy player setting to the camera
	camera.setPosition(player.position);
	camera.setRotation(player.horizontalRotation, player.verticalRotation);
}