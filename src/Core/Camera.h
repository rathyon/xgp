#ifndef __XGP_CAMERA_H__
#define __XGP_CAMERA_H__

#include "SceneObject.h"

namespace xgp {

	struct CameraData {
		glm::mat4 viewMatrix;
		glm::mat4 projMatrix;
		glm::mat4 viewProjMatrix;
		glm::vec3 viewPos;
	};

	class Camera : public SceneObject {
	public:
		Camera(int width, int height, const glm::vec3& eye,
			const glm::vec3& at, const glm::vec3& up, float near, float far);

		int width() const;
		int height() const;

		float aspect() const;
		float near() const;
		float far() const;

		glm::vec3 right() const;
		glm::vec3 front() const;
		glm::vec3 up() const;

		glm::mat4 viewMatrix() const;
		glm::mat4 projMatrix() const;

		glm::mat4 viewProjMatrix() const;

		void updateOrientation(float dp, float dy);

		void updateViewMatrix();
		virtual void updateProjMatrix(int width, int height);

	protected:
		int _width;
		int _height;

		float _near;
		float _far;

		glm::mat4 _projMatrix;

		float _pitch; // radians
		float _yaw; // radians
	};

}

#endif