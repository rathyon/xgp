#include "XGPApp.h"

using namespace xgp;


/*
	TODO:

	Perspective class
	Model class
	Material class
	Scene class

*/


XGPApp* app;

void errorCallback(int error, const char* description) {
	app->errorCallback(error, description);
}
void reshapeCallback(GLFWwindow* window, int width, int height) {
	app->reshapeCallback(width, height);
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	app->keyCallback(key, scancode, action, mods);
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	app->mouseButtonCallback(button, action, mods);
}
void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
	app->mousePosCallback(xpos, ypos);
}

int main() {
	app = new XGPApp("XGP - eXperimental Graphics Program", 640, 480);

	app->init();

	//Set callbacks
	glfwSetErrorCallback(errorCallback);
	glfwSetFramebufferSizeCallback(app->window(), reshapeCallback);
	glfwSetKeyCallback(app->window(), keyCallback);
	glfwSetMouseButtonCallback(app->window(), mouseButtonCallback);
	glfwSetCursorPosCallback(app->window(), mousePosCallback);

	app->loop();

	app->cleanup();
	delete app;

	exit(EXIT_SUCCESS);
}
