#include "Perspective.h"

using namespace xgp;

Perspective::Perspective(int width, int height, const glm::vec3& eye,
	const glm::vec3& at, const glm::vec3& up, float near, float far, float fov)
	: Camera(width, height, eye, at, up, near, far), _fov(fov) {

	_projMatrix = glm::perspective(glm::radians(_fov), aspect(), _near, _far);
}

float Perspective::fov() const {
	return _fov;
}

void Perspective::updateProjMatrix(int width, int height) {
	Camera::updateProjMatrix(width, height);

	_projMatrix = glm::perspective(glm::radians(_fov), aspect(), _near, _far);
}