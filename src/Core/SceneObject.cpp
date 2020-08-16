#include "SceneObject.h"

using namespace xgp;

SceneObject::SceneObject()
	: _position(glm::vec3(0)), _scale(glm::vec3(1)), _rotation(glm::quat(glm::vec3(0))), _parent(nullptr) { }

SceneObject::SceneObject(const glm::vec3& position)
	: _position(position), _scale(glm::vec3(1)), _rotation(glm::quat(glm::vec3(0))), _parent(nullptr) { }

SceneObject::SceneObject(const glm::mat4& objToWorld)
	: _objToWorld(objToWorld), _parent(nullptr){

	_position = glm::vec3(_objToWorld[0][3],
						  _objToWorld[1][3],
						  _objToWorld[2][3]);
}

const glm::vec3& SceneObject::position()    const {
	return _position;
}

const glm::vec3& SceneObject::scale()       const {
	return _scale;
}

const glm::quat& SceneObject::rotation() const {
	return _rotation;
}

const glm::mat4& SceneObject::objToWorld() const {
	return _objToWorld;
}

std::shared_ptr<SceneObject> SceneObject::parent() const {
	return _parent;
}

void SceneObject::setPosition(const glm::vec3& position) {
	_position = position;
}

void SceneObject::setScale(float x, float y, float z) {
	_scale = glm::vec3(x, y, z);
}

void SceneObject::setRotation(const glm::quat& quat) {
	_rotation = quat;
}

void SceneObject::setObjToWorld(const glm::mat4& mat) {
	_objToWorld = mat;
}

void SceneObject::updateMatrix() {
	_objToWorld = glm::translate(glm::mat4(1.0f), _position) *
		glm::mat4_cast(_rotation) *
		glm::scale(glm::mat4(1.0f), _scale);
}