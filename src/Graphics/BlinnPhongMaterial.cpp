#include "BlinnPhongMaterial.h"

#include <Resources.h>
#include <Shader.h>

using namespace xgp;

BlinnPhongMaterial::BlinnPhongMaterial() {
    _program = Resource.getShader("blinnphong")->id();

    _ambient = glm::vec3(-1.0f);
    _diffuse = glm::vec3(-1.0f);
    _specular = glm::vec3(-1.0f);
    _shininess = 1.0f;

    _diffuseTex = GL_FALSE;
    _specularTex = GL_FALSE;
    _normalMap = GL_FALSE;
    _heightMap = GL_FALSE;

    _envMap = GL_FALSE;
}

void BlinnPhongMaterial::uploadData() {
    // set shader variables
    setVec3("ambient", _ambient);
    setVec3("diffuse", _diffuse);
    setVec3("specular", _specular);
    setFloat("shininess", _shininess);

    if (_diffuseTex != GL_FALSE) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _diffuseTex);
        setSampler("diffuseTex", 0);
    }

    if (_specularTex != GL_FALSE) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _specularTex);
        setSampler("specularTex", 1);
    }

    if (_normalMap != GL_FALSE) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, _normalMap);
        setSampler("normalMap", 2);
    }

    if (_heightMap != GL_FALSE) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, _heightMap);
        setSampler("heightMap", 3);
    }

    if (_envMap != GL_FALSE) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _envMap);
        setSampler("envMap", 4);
    }
}

void BlinnPhongMaterial::update(const Skybox& skybox) {
    _envMap = skybox.envMap();
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

GLuint BlinnPhongMaterial::envMap() const {
    return _envMap;
}