#ifndef __XGP_POINTLIGHT_H__
#define __XGP_POINTLIGHT_H__

#include "Light.h"

namespace xgp {

	class PointLight : public Light {
	public:
		PointLight();
		PointLight(const glm::vec3& emission);
		PointLight(const glm::vec3& emission, const glm::vec3& position);

		void toData(LightData& data) const override;
	};

}

#endif
