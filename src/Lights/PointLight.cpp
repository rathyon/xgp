#include "PointLight.h"

using namespace xgp;

PointLight::PointLight() : Light() { }

PointLight::PointLight(const glm::vec3& emission)
	: Light(emission) {
}

PointLight::PointLight(const glm::vec3& emission, const glm::vec3& position)
	: Light(emission, position) {
}

void PointLight::toData(LightData& data) const {
	data.position = _position;
	data.direction = glm::vec3(0);
	data.emission = _emission;
	data.type = LightType::LIGHTYPE_POINT;
	data.state = _on;
	data.cutoff = 0.0f;
}