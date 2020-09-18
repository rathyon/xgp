#include "XGPApp.h"

#include <iostream>

#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "BlinnPhongMaterial.h"
#include "Perspective.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Resources.h"
#include <Utils.h>

#include <glm/gtx/string_cast.hpp>

using namespace xgp;

const std::string SHADERS_DIR = "../../src/Shaders/";
const std::string MODELS_DIR = "../../assets/models/";
const std::string IMAGES_DIR = "../../assets/images/";

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

void XGPApp::loadShaders() {
	ShaderSource vert = ShaderSource(GL_VERTEX_SHADER, SHADERS_DIR + "flat.vs");
	ShaderSource frag = ShaderSource(GL_FRAGMENT_SHADER, SHADERS_DIR + "flat.fs");
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("test");
	shader->addShader(vert);
	shader->addShader(frag);
	shader->link();
	Resource.addShader("test", shader);
	_shaders.push_back(shader);
}

void XGPApp::loadImages() {
	Image _diffuseImg = Image();
	_diffuseImg.loadImage(IMAGES_DIR + "Metal_tiles_002_SD/Metal_Tiles_002_basecolor.jpg", IMG_2D);
	std::shared_ptr<Texture> _diffuseMap = std::make_shared<Texture>(_diffuseImg);
	Resource.addTexture("diffuseMap", _diffuseMap);

	Image _normalImg = Image();
	_normalImg.loadImage(IMAGES_DIR + "Metal_tiles_002_SD/Metal_Tiles_002_normal.jpg", IMG_2D);
	std::shared_ptr<Texture> _normalMap = std::make_shared<Texture>(_normalImg);
	Resource.addTexture("normalMap", _normalMap);
}

void XGPApp::loadModels() {
	std::shared_ptr<BlinnPhongMaterial> mat;
	mat = std::make_shared<BlinnPhongMaterial>();
	mat->setProgram(Resource.getShader("test")->id());
	mat->setDiffuseTex(Resource.getTexture("diffuseMap")->id());
	mat->setNormalMap(Resource.getTexture("normalMap")->id());
	mat->setSpecular(glm::vec3(1.0f));
	mat->setShininess(64.f);

	std::shared_ptr<Model> model = std::make_shared<Model>(MODELS_DIR + "cube.obj");
	model->prepare();
	model->updateMatrix();
	model->setMaterial(mat);
	Resource.addShape("model", model);
	_scene.addShape(model);
}


void XGPApp::prepare() {
	Resource.initialize();

	// Camera
	_camera = new Perspective(_width, _height, glm::vec3(3.f, 3.f, 3.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f), 0.1f, 1000.f, 90.f);

	// Lights
	std::shared_ptr<DirectionalLight> light1 = std::make_shared<DirectionalLight>(glm::vec3(1), glm::vec3(0.0f, -1.0f, -1.0f));
	_scene.addLight(light1);

	loadShaders();
	loadImages();
	loadModels();

	// Buffers to the GPU
	glGenBuffers(1, &_cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, _cameraBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), 0, GL_DYNAMIC_DRAW);
	// Set block binding for all shaders -> OpenGL 4.2 and onwards allows this to be done in the shader!
	for(std::shared_ptr<Shader> shader : _shaders) {
		glUniformBlockBinding(shader->id(), glGetUniformBlockIndex(shader->id(), "cameraBlock"), CAMERA_BUFFER_IDX);
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_BUFFER_IDX, _cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glGenBuffers(1, &_lightsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, _lightsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightData) * _scene.lights().size(), 0, GL_DYNAMIC_DRAW);
	for (std::shared_ptr<Shader> shader : _shaders) {
		glUniformBlockBinding(shader->id(), glGetUniformBlockIndex(shader->id(), "lightBlock"), LIGHTS_BUFFER_IDX);
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHTS_BUFFER_IDX, _lightsBuffer);
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
	// Camera look
	const float CAMERA_LOOK_SPEED = 0.7f;
	if (_mouseBtn[GLFW_MOUSE_BUTTON_LEFT]) {
		_camera->updateOrientation(-_mouseDy * dt * CAMERA_LOOK_SPEED, -_mouseDx * dt * CAMERA_LOOK_SPEED);
		_camera->updateViewMatrix();
	}

	_mouseDx = 0; _mouseDy = 0;

	// Camera movement
	glm::vec3 moveDir = glm::vec3(0);
	if (_keys[GLFW_KEY_W]) {
		moveDir += -_camera->front();
	}
	else if (_keys[GLFW_KEY_S]) {
		moveDir += _camera->front();
	}
	if (_keys[GLFW_KEY_D]) {
		moveDir += _camera->right();
	}
	else if (_keys[GLFW_KEY_A]) {
		moveDir += -_camera->right();
	}

	const float CAMERA_MOVEMENT_SPEED = 7.0f;
	if (moveDir != glm::vec3(0)) {
		_camera->setPosition(_camera->position() + glm::normalize(moveDir) * (float)dt * CAMERA_MOVEMENT_SPEED);
		_camera->updateViewMatrix();
	}
}

void XGPApp::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	uploadCameraData();
	uploadLightData();

	for (std::shared_ptr<Shape> shape : _scene.shapes()) {
		shape->draw();
	}
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

	if (action == GLFW_PRESS)
		_keys[key] = true;
	else if (action == GLFW_RELEASE)
		_keys[key] = false;
}

void XGPApp::mouseButtonCallback(int button, int action, int mods) {
	if (action == GLFW_PRESS)
		_mouseBtn[button] = true;
	else if (action == GLFW_RELEASE)
		_mouseBtn[button] = false;
}

void XGPApp::mousePosCallback(double xpos, double ypos) {
	int dx = xpos - _mouseX;
	int dy = _mouseY - ypos;

	_mouseX = xpos;
	_mouseY = ypos;

	_mouseDx = dx;
	_mouseDy = dy;
}

void XGPApp::uploadCameraData() {
	CameraData data;
	data.viewMatrix = _camera->viewMatrix();
	data.projMatrix = _camera->projMatrix();
	data.viewProjMatrix = _camera->viewProjMatrix();
	data.viewPos = _camera->position();

	glBindBuffer(GL_UNIFORM_BUFFER, _cameraBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), &data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void XGPApp::uploadLightData() {
	glBindBuffer(GL_UNIFORM_BUFFER, _lightsBuffer);
	for (int i = 0; i < _scene.lights().size(); i++) {
		LightData data;
		_scene.lights()[i]->toData(data);
		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(LightData), sizeof(LightData), &data);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}