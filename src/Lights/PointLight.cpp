#include "PointLight.h"

using namespace xgp;

PointLight::PointLight() : Light() { }

PointLight::PointLight(const glm::vec3& emission)
	: Light(emission) {
	
	// approximate values for MOST cases
	_linearAttenuation = 0.1f;
	_quadraticAttenuation = 0.032f;
}

PointLight::PointLight(const glm::vec3& emission, const glm::vec3& position)
	: Light(emission, position) {

	// approximate values for MOST cases
	_linearAttenuation = 0.1f;
	_quadraticAttenuation = 0.032f;
}

PointLight::PointLight(const glm::vec3& emission, const glm::vec3& position, float linear, float quadratic)
	: Light(emission, position), _linearAttenuation(linear), _quadraticAttenuation(quadratic) { }

void PointLight::toData(LightData& data) const {
	data.position = _position;
	data.direction = glm::vec3(0);
	data.emission = _emission;
	data.linear = _linearAttenuation;
	data.quadratic = _quadraticAttenuation;
	data.type = LightType::LIGHTYPE_POINT;
	data.state = _on;
	data.cutoff = 0.0f;
}