#ifndef __XGP_POINTLIGHT_H__
#define __XGP_POINTLIGHT_H__

#include "Light.h"

namespace xgp {

	class PointLight : public Light {
	public:
		PointLight();
		PointLight(const glm::vec3& emission);
		PointLight(const glm::vec3& emission, const glm::vec3& position);
		PointLight(const glm::vec3& emission, const glm::vec3& position, float linearAttenuation, float quadraticAttenuation);

		void toData(LightData& data) const override;

	private:
		float _linearAttenuation;
		float _quadraticAttenuation;
	};

}

#endif
