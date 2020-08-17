#ifndef __XGP_LIGHT_H__
#define __XGP_LIGHT_H__

#include <GL/glew.h>

#include <SceneObject.h>

namespace xgp {

	// Used to pass the type id to shaders
	enum LightType : int {
		LIGHTYPE_DIR   = 0,
		LIGHTYPE_POINT = 1,
		LIGHTYPE_SPOT  = 2,
	};

	// Light data for shader blocks
	// CARE! Data is properly aligned, make sure struct is same in shader
	struct LightData {
		glm::vec3 position;
		float auxA;            // padding
		glm::vec3 direction;   // for directional and spot lights
		float auxB;            // padding
		glm::vec3 emission;    // "color" of the light
		float auxC;            // padding
		float linear;	       // attenuation
		float quadratic;       // attenuation
		int type;
		bool state;            // on or off
		float cutoff;          // stored as cosine
	};

	class Light : public SceneObject {
	public:
		Light();
		Light(const glm::vec3& emission);
		Light(const glm::vec3& emission, const glm::vec3& position);

		bool isOn() const;
		bool castingShadows() const;
		glm::vec3 emission() const;

		void setOn(bool on);
		void setCastShadows(bool val);
		void setEmission(glm::vec3& emission);

		virtual void toData(LightData& data) const = 0;

	protected:
		bool _on;
		bool _shadows;
		glm::vec3 _emission;
	};

}

#endif
