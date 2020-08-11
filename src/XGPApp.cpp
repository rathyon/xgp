#include "XGPApp.h"

#include <iostream>

using namespace xgp;

XGPApp::XGPApp(const std::string title, int width, int height)
	: _title(title), _width(width) , _height(height), _window(nullptr) {
}

void XGPApp::init() {
	// Initialize GLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//My machine supports up to OpenGL 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//Initialize GLEW
	glfwMakeContextCurrent(_window);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "ERROR: " << glewGetErrorString(err) << std::endl;
	}

	// Print system info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;

	// Initialize OpenGL state
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glDepthMask(GL_TRUE);
	//glDepthRange(0.0, 1.0);
	//glClearDepth(1.0);
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glEnable(GL_MULTISAMPLE);

	int fwidth, fheight;
	glfwGetFramebufferSize(_window, &fwidth, &fheight);
	glViewport(0, 0, fwidth, fheight);

	// time since GLFW was initialized, in seconds
	_oldTime = glfwGetTime();
}

void XGPApp::loop() {
	while (!glfwWindowShouldClose(_window)) {
		double newTime = glfwGetTime();
		double dt = newTime - _oldTime;
		_oldTime = newTime;

		// Limit the delta time to avoid large intervals
		if (dt > 0.25f)
			dt = 0.25f;

		update(dt);
		render();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void XGPApp::update(double dt) {

}

void XGPApp::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void XGPApp::setTitle(const std::string& title) {
	_title = title;
}

void XGPApp::cleanup() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

GLFWwindow* XGPApp::window() const {
	return _window;
}

void XGPApp::errorCallback(int error, const char* description) {
	std::cerr << "ERROR: " << description << std::endl;
}

void XGPApp::reshapeCallback(int width, int height) {
	glViewport(0, 0, width, height);
}

void XGPApp::keyCallback(int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

void XGPApp::mouseButtonCallback(int button, int action, int mods) {

}

void XGPApp::mousePosCallback(double xpos, double ypos) {

}
