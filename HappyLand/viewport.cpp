#include "viewport.h"

#include <GL/glew.h>

viewport::viewport(unsigned int _width, unsigned int _height) : width(_width), height(_height) {
	// -- Initialize everything --
	// Window
	if (!glfwInit())
		throw "Couldn't start GLFW";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Window!!!", 0, 0);
	if (!window) {
		glfwTerminate();
		throw "Couldn't open a window";
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	
	// Set OpenGL variables
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Set callbacks
	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, resizeCallback);
	resizeCallback(window, width, height);
	glfwSetScrollCallback(window, scrollCallback);
	scrollCallback(window, 0, 0);

	// Set mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


viewport::~viewport() {
	glfwTerminate();
}

void viewport::resizeCallback(GLFWwindow *window, int w, int h) {
	viewport &me = *((viewport*) glfwGetWindowUserPointer(window));

	// Handle resizing
	me.width = w;
	me.height = h;
	glViewport(0, 0, w, h);
}
#include <iostream>
using namespace std;
void viewport::scrollCallback(GLFWwindow *window, double x, double y) {
	viewport &me = *((viewport*) glfwGetWindowUserPointer(window));

	// Handle scrolling
	me.mouseScrollDelta = math::vec2((float) x, (float) y);
}