#include "XGPApp.h"

#include <iostream>

#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "BlinnPhongMaterial.h"
#include "Perspective.h"
#include <Utils.h>

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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	prepare();
}

//temporary vars for testing
Model* _model;
std::shared_ptr<BlinnPhongMaterial> _mat;
Perspective* _camera;
GLuint _cameraBuffer;


void XGPApp::prepare() {
	// Temporary test setup
	ShaderSource vert = ShaderSource(GL_VERTEX_SHADER, "../../src/Shaders/flat.vs");
	ShaderSource frag = ShaderSource(GL_FRAGMENT_SHADER, "../../src/Shaders/flat.fs");
	Shader _shader = Shader("test");
	_shader.addShader(vert);
	_shader.addShader(frag);
	_shader.link();

	_model = new Model("../../assets/models/cube.obj");
	_model->prepare();
	_model->updateMatrix();

	_mat = std::make_shared<BlinnPhongMaterial>();
	_mat->setProgram(_shader.id());

	_model->setMaterial(_mat);

	Image _texImg = Image();
	_texImg.loadImage("../../assets/models/diffuse.png", IMG_2D);
	Texture _tex = Texture(_texImg);

	_camera = new Perspective(_width, _height, glm::vec3(3.f, 3.f, 3.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f), 0.1f, 1000.f, 90.f);

	// Buffers to the GPU
	glGenBuffers(1, &_cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, _cameraBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), 0, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, _cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

	// Upload camera
	CameraData data;
	data.viewMatrix = _camera->viewMatrix();
	data.projMatrix = _camera->projMatrix();
	data.viewProjMatrix = _camera->viewProjMatrix();
	data.viewPos = _camera->position();

	glBindBuffer(GL_UNIFORM_BUFFER, _cameraBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), &data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	_model->draw();
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
	_width = width;
	_height = height;
	_camera->updateProjMatrix(width, height);
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
