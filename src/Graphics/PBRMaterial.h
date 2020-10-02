#ifndef __XGP_PBRMATERIAL_H__
#define __XGP_PBRMATERIAL_H__

#include "Material.h"
#include "Image.h"

namespace xgp {

    class PBRMaterial : public Material {
    public:
		PBRMaterial();

		void uploadData();
		void update(const Skybox& skybox);

		void setAlbedo(const glm::vec3& albedo);
		void setAlbedoMap(GLuint albedoTex);
		void setMetallic(float metal);
		void setMetallicMap(GLuint metalMap);
		void setRoughness(float rough);
		void setRoughnessMap(GLuint roughMap);
		void setNormalMap(GLuint normMap);

		glm::vec3 albedo() const;
		float metallic() const;
		float roughness() const;

		GLuint albedoMap() const;
		GLuint metallicMap() const;
		GLuint roughnessMap() const;
		GLuint normalMap() const;

		GLuint irradianceMap() const;
		GLuint prefilterMap() const;
		GLuint brdfLUT() const;

    private:
        glm::vec3 _albedo;
		float _metallic;
        float _roughness;
		const float _f0 = 0.04f; //base reflectivity of most dielectrics

        GLuint _albedoMap;
        GLuint _metallicMap;
		GLuint _roughnessMap;
        GLuint _normalMap;

		GLuint _irradianceMap;
		GLuint _prefilterMap;
		GLuint _brdfLUT;
    };

}

#endif