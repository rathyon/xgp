#include "Light.h"

using namespace xgp;

Light::Light() : SceneObject(), _on(true), _emission(glm::vec3(1.0f)) { }

Light::Light(const glm::vec3& emission)
	: SceneObject(), _on(true), _emission(emission) { }

Light::Light(const glm::vec3& emission, const glm::vec3& position)
	: SceneObject(position), _on(true), _emission(emission) { }

bool Light::isOn() const {
	return _on;
}

bool Light::castingShadows() const {
	return _shadows;
}

glm::vec3 Light::emission() const {
	return _emission;
}

void Light::setOn(bool on) {
	_on = on;
}

void Light::setCastShadows(bool val) {
	_shadows = val;
}

void Light::setEmission(glm::vec3& emission) {
	_emission = emission;
}