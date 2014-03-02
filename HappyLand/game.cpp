#include "game.h"

#include <sstream>

game::game(::viewport &v) :
	viewport(v), camera(),
	entityManager(), playerManager(*this, camera),
	entityRenderManager(entityManager, camera) {
}


game::~game() {
}

void game::init() {
	modelLoader mloader("models");
	model monkey = mloader.load("model.dat", "model_diffuce.png", "model_specular.png");
	//model test = mloader.load("out.dat", "monkey_diffuse2.png");
	//entityManager.add(new modelEntity(*this, test, math::vec3(0, -2, 0)));
	entityManager.add(new modelEntity(*this, monkey, math::vec3()));
	//for (int i = -10; i <= 10; i++)
	//	entityManager.add(new modelEntity(*this, monkey, math::vec3(3.0f * i, 0, 0)));
}

void game::run() {
	double lastUpdate = glfwGetTime();

	//glClearColor(.2, .2, .2, 0);
	while (1) {
		double now = glfwGetTime();
		double delta = now - lastUpdate;
		lastUpdate = now;
		if (delta > 0.05) delta = 0.05;		// Don't allow delta to go to high
		stringstream ss;
		ss << (1/delta);
		viewport.setTitle(ss.str());

		// Input
		if (viewport.shouldClose() || isKeyPressed(GLFW_KEY_ESCAPE)) {
			// Close game
			break;
		}

		// Update
		// Set camera (might be needed for some updates)
		camera.resize(viewport.getWidth(), viewport.getHeight());
		camera.update();

		// Update all entities
		entityManager.update(delta);
		playerManager.update(delta);

		// Render
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// TODO Move to renderer
		entityRenderManager.render();
		viewport.flip();
	}
}

void game::stop() {
}
