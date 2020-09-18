#include "BlinnPhongMaterial.h"

#include <Resources.h>

using namespace xgp;

BlinnPhongMaterial::BlinnPhongMaterial() {
    _ambient = glm::vec3(0.0f);
    _diffuse = glm::vec3(-1.0f);
    _specular = glm::vec3(-1.0f);
    _shininess = 0.0f;

    _diffuseTex = -1;
    _specularTex = -1;
    _normalMap = -1;
    _heightMap = -1;
}

void BlinnPhongMaterial::uploadData() {
    // set shader variables
    // TODO:
    // test if setting a value of -1 is usable for checking presence of textures (instead of 0)
    setVec3("ambient", _ambient);
    setVec3("diffuse", _diffuse);
    setVec3("specular", _specular);
    setFloat("shininess", _shininess);

    if (_diffuseTex != -1) {
        glActiveTexture(GL_TEXTURE0 + TextureUnit::DIFFUSE_MAP);
        glBindTexture(GL_TEXTURE_2D, _diffuseTex);
        setSampler("diffuseMap", TextureUnit::DIFFUSE_MAP);
    }

    if (_specularTex != -1) {
        glActiveTexture(GL_TEXTURE0 + TextureUnit::SPECULAR_MAP);
        glBindTexture(GL_TEXTURE_2D, _specularTex);
        setSampler("specularMap", TextureUnit::SPECULAR_MAP);
    }

    if (_normalMap != -1) {
        glActiveTexture(GL_TEXTURE0 + TextureUnit::NORMAL_MAP);
        glBindTexture(GL_TEXTURE_2D, _normalMap);
        setSampler("normalMap", TextureUnit::NORMAL_MAP);
    }

    if (_heightMap != -1) {
        glActiveTexture(GL_TEXTURE0 + TextureUnit::HEIGHT_MAP);
        glBindTexture(GL_TEXTURE_2D, _heightMap);
        setSampler("heightMap", TextureUnit::HEIGHT_MAP);
    }
}

void BlinnPhongMaterial::setAmbient(const glm::vec3& ambient) {
    _ambient = ambient;
}

void BlinnPhongMaterial::setDiffuse(const glm::vec3& diffuse) {
    _diffuse = diffuse;
}

void BlinnPhongMaterial::setDiffuseTex(GLuint diffTex) {
    _diffuseTex = diffTex;
}

void BlinnPhongMaterial::setSpecular(const glm::vec3& specular) {
    _specular = specular;
}

void BlinnPhongMaterial::setSpecularTex(GLuint specTex) {
    _specularTex = specTex;
}

void BlinnPhongMaterial::setNormalMap(GLuint normMap) {
    _normalMap = normMap;
}

void BlinnPhongMaterial::setHeightMap(GLuint heightMap) {
    _heightMap = heightMap;
}

void BlinnPhongMaterial::setShininess(float shininess) {
    _shininess = shininess;
}

glm::vec3 BlinnPhongMaterial::ambient() const {
    return _ambient;
}
glm::vec3 BlinnPhongMaterial::diffuse() const {
    return _diffuse;
}
glm::vec3 BlinnPhongMaterial::specular() const {
    return _specular;
}

float BlinnPhongMaterial::shininess() const {
    return _shininess;
}

GLuint BlinnPhongMaterial::diffuseTex() const {
    return _diffuseTex;
}
GLuint BlinnPhongMaterial::specularTex() const {
    return _specularTex;
}
GLuint BlinnPhongMaterial::normalMap() const {
    return _normalMap;
}

GLuint BlinnPhongMaterial::heightMap() const {
    return _heightMap;
}