#ifndef __XGP_XGPAPP_H__
#define __XGP_XGPAPP_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include <Camera.h>
#include <Scene.h>
#include <Shader.h>
#include <Skybox.h>

namespace xgp {

	enum BufferIndices {
		CAMERA_BUFFER_IDX = 0,
		LIGHTS_BUFFER_IDX = 1,
	};

	class XGPApp {
	public:
		XGPApp(const std::string title, int width, int height);

		void init();
		void loop();
		void update(double dt);
		void render();
		void setTitle(const std::string& title);
		void cleanup();

		GLFWwindow* window() const;

		void errorCallback(int error, const char* description);
		void reshapeCallback(int width, int height);
		void keyCallback(int key, int scancode, int action, int mods);
		void mouseButtonCallback(int button, int action, int mods);
		void mousePosCallback(double xpos, double ypos);

	private:
		void prepare();
		void loadShaders();
		void loadImages();
		void loadModels();

		void changeSkybox(int id);

		void uploadCameraData();
		void uploadLightData();

		GLFWwindow* _window;
		int _width, _height;
		std::string _title;
		double _oldTime;
		bool _keys[256];
		bool _mouseBtn[8];
		int _mouseX, _mouseY;
		float _mouseDx, _mouseDy;

		Scene _scene;
		Camera* _camera;
		GLuint _cameraBuffer;
		GLuint _lightsBuffer;
		std::vector<std::shared_ptr<Shader>> _shaders;
		std::vector<Skybox> _skyboxes;
	};
}

#endif