#ifndef __XGP_SPOTLIGHT_H__
#define __XGP_SPOTLIGHT_H__

#include "Light.h"

namespace xgp {

	class SpotLight : public Light {
	public:
		SpotLight();
		SpotLight(const glm::vec3& emission, float cutoff);
		SpotLight(const glm::vec3& emission, float cutoff, const glm::vec3& direction);
		SpotLight(const glm::vec3& emission, float cutoff, const glm::vec3& direction, const glm::vec3& position);
		SpotLight(const glm::vec3& emission, float cutoff, const glm::vec3& direction, const glm::vec3& position, float linearAttenuation, float quadraticAttenuation);

		glm::vec3 direction() const;
		float cutoff() const;

		void toData(LightData& data) const override;

	private:
		float _linearAttenuation;
		float _quadraticAttenuation;

		glm::vec3 _direction;
		// cutoff is passed in constructor in degrees
		// but it is stored as cosine(cutoff)
		float _cutoff;
	};

}

#endif // !__RSM_POINTLIGHT_H__
