#ifndef __XGP_BLINNPHONGMATERIAL_H__
#define __XGP_BLINNPHONGMATERIAL_H__

#include "Material.h"
#include "Texture.h"

namespace xgp {

    class BlinnPhongMaterial : public Material {
    public:
        BlinnPhongMaterial();

		void uploadData();

		void setAmbient(const glm::vec3& ambient);

		void setDiffuse(const glm::vec3& diffuse);
		void setDiffuseTex(GLuint diffTex);

		void setSpecular(const glm::vec3& specular);
		void setSpecularTex(GLuint specTex);

		void setNormalMap(GLuint normalMap);
		void setHeightMap(GLuint heightMap);

		void setShininess(float shininess);

		glm::vec3 ambient() const;
		glm::vec3 diffuse() const;
		glm::vec3 specular() const;
		float shininess() const;

		GLuint diffuseTex() const;
		GLuint specularTex() const;
		GLuint normalMap() const;
		GLuint heightMap() const;

    private:
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
        float _shininess;

        GLuint _diffuseTex;
        GLuint _specularTex;
        GLuint _normalMap;
		GLuint _heightMap;
    };

}


#endif