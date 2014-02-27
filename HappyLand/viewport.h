#pragma once

class renderer;

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <math/vec.h>
#include <string>

class camera;
class viewport {
public:
	viewport(unsigned int _width, unsigned int _height);
	~viewport();

	void flip() {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	int shouldClose() {
		int res = glfwWindowShouldClose(window);
		glfwSetWindowShouldClose(window, 0);
		return res;
	}
	
	unsigned int getWidth() const {
		return width;
	}
	unsigned int getHeight() const {
		return height;
	}

	bool isKeyPressed(int key) const {
		return glfwGetKey(window, key) != GLFW_RELEASE;
	}

	math::vec2 getMouseDelta() const {
		double mx, my;
		glfwGetCursorPos(window, &mx, &my);
		math::vec2 currentPos((float) mx, (float) my);
		math::vec2 mouseDelta = currentPos - lastMousePos;
		lastMousePos = currentPos;
		return mouseDelta;
	}

	math::vec2 getMouseScrollDelta() const {
		math::vec2 result = mouseScrollDelta;
		mouseScrollDelta = math::vec2();
		return result;
	}

	void setTitle(std::string title) {
		glfwSetWindowTitle(window, title.c_str());
	}

private:
	// Prevent copying
	viewport(const viewport&);
	viewport& operator=(const viewport&);

	GLFWwindow *window;
	unsigned int width, height;

	bool isCursorHooked;
	mutable math::vec2 lastMousePos;
	mutable math::vec2 mouseScrollDelta;

	static void resizeCallback(GLFWwindow *window, int w, int h);
	static void scrollCallback(GLFWwindow *window, double dx, double dy);
};

