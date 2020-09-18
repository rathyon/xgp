#include "Camera.h"

using namespace xgp;

Camera::Camera(int width, int height, const glm::vec3& eye,
	const glm::vec3& at, const glm::vec3& up, float near, float far)
	: _width(width), _height(height), _near(near), _far(far){

	_position = eye;
	_objToWorld = glm::lookAt(eye, at, up);
	glm::mat4 matOrient = _objToWorld;

	//last column
	matOrient[0][3] = 0;
	matOrient[1][3] = 0;
	matOrient[2][3] = 0;
	matOrient[3][3] = 1;

	_rotation = glm::quat(matOrient);

	glm::vec3 viewDirection = glm::normalize(at - eye);
	_pitch = asinf(-viewDirection.y);
	_yaw = atan2f(viewDirection.x, -viewDirection.z);
}

int Camera::width() const {
	return _width;
}

int Camera::height() const {
	return _height;
}

float Camera::aspect() const {
	return (float)_width / (float)_height;
}

float Camera::near() const {
	return _near;
}

float Camera::far() const {
	return _far;
}

glm::vec3 Camera::right() const {
	return glm::vec3(_objToWorld[0][0],
					_objToWorld[1][0],
					_objToWorld[2][0]);
}

glm::vec3 Camera::up() const {
	return glm::vec3(_objToWorld[0][1],
					_objToWorld[1][1],
					_objToWorld[2][1]);
}

glm::vec3 Camera::front() const {
	return glm::vec3(_objToWorld[0][2],
					_objToWorld[1][2],
					_objToWorld[2][2]);
}

glm::mat4 Camera::viewMatrix() const {
	return _objToWorld;
}

glm::mat4 Camera::projMatrix() const {
	return  _projMatrix;
}

glm::mat4 Camera::viewProjMatrix() const {
	return projMatrix() * viewMatrix();
}

void Camera::updateOrientation(float dp, float dy) {
	_pitch += dp;
	_yaw -= dy;

	_yaw = fmodf(_yaw, glm::two_pi<float>());

	if (_yaw < 0.0f)
		_yaw += glm::two_pi<float>();

	if (_pitch > glm::half_pi<float>())
		_pitch = glm::half_pi<float>();
	else if (_pitch < -glm::half_pi<float>())
		_pitch = -glm::half_pi<float>();
}

void Camera::updateViewMatrix() {
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), _pitch, glm::vec3(1, 0, 0));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), _yaw, glm::vec3(0, 1, 0));

	glm::mat4 orientation = rotX * rotY;

	_objToWorld = orientation * glm::translate(glm::mat4(1),  -_position);
}

void Camera::updateProjMatrix(int width, int height) {
	_width = width;
	_height = height;
}