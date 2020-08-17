#ifndef __XGP_DIRECTIONALLIGHT_H__
#define __XGP_DIRECTIONALLIGHT_H__

#include "Light.h"

namespace xgp {

	class DirectionalLight : public Light {
	public:
		DirectionalLight();
		DirectionalLight(const glm::vec3& emission);
		DirectionalLight(const glm::vec3& emission, const glm::vec3& direction);

		glm::vec3 direction() const;

		void toData(LightData& data) const override;

	private:
		glm::vec3 _direction;
	};

}

#endif
