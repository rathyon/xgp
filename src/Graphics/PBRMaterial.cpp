#include "PBRMaterial.h"

#include <Resources.h>
#include <Shader.h>

using namespace xgp;

PBRMaterial::PBRMaterial() {
    _program = Resource.getShader("unreal")->id();

    _brdfLUT = Resource.getImage("brdfLUT")->id();

    _albedo = glm::vec3(-1.0f);
    _metallic = -1.0f;
    _roughness = -1.0f;

    _albedoMap = GL_FALSE;
    _metallicMap = GL_FALSE;
    _roughnessMap = GL_FALSE;
    _normalMap = GL_FALSE;

    _irradianceMap = GL_FALSE;
    _prefilterMap = GL_FALSE;
}

void PBRMaterial::uploadData() {
    // set shader variables
    setVec3("albedo", _albedo);
    setFloat("metallic", _metallic);
    setFloat("roughness", _roughness);
    setFloat("f0", _f0);

    if (_albedoMap != GL_FALSE) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _albedoMap);
        setSampler("albedoMap", 0);
    }

    if (_metallicMap != GL_FALSE) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _metallicMap);
        setSampler("metallicMap", 1);
    }

    if (_roughnessMap != GL_FALSE) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, _roughnessMap);
        setSampler("roughnessMap", 2);
    }

    if (_normalMap != GL_FALSE) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, _normalMap);
        setSampler("normalMap", 3);
    }

    if (_irradianceMap != GL_FALSE) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _irradianceMap);
        setSampler("irradianceMap", 4);
    }

    if (_prefilterMap != GL_FALSE) {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _prefilterMap);
        setSampler("prefilterMap", 5);
    }

    if (_brdfLUT != GL_FALSE) {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, _brdfLUT);
        setSampler("brdfLUT", 6);
    }
}

void PBRMaterial::update(const Skybox& skybox) {
    _irradianceMap = skybox.irradianceMap();
    _prefilterMap = skybox.ggxPrefilterMap();
}

void PBRMaterial::setAlbedo(const glm::vec3& albedo) {
    _albedo = albedo;
}

void PBRMaterial::setAlbedoMap(GLuint albedoMap) {
    _albedoMap = albedoMap;
}

void PBRMaterial::setMetallic(float metal) {
    _metallic = metal;
}

void PBRMaterial::setMetallicMap(GLuint metalMap) {
    _metallicMap = metalMap;
}

void PBRMaterial::setRoughness(float rough) {
    _roughness = rough;
}

void PBRMaterial::setRoughnessMap(GLuint roughMap) {
    _roughnessMap = roughMap;
}

void PBRMaterial::setNormalMap(GLuint normMap) {
    _normalMap = normMap;
}

glm::vec3 PBRMaterial::albedo() const {
    return _albedo;
}

float PBRMaterial::metallic() const {
    return _metallic;
}

float PBRMaterial::roughness() const {
    return _roughness;
}

GLuint PBRMaterial::albedoMap() const {
    return _albedoMap;
}

GLuint PBRMaterial::metallicMap() const {
    return _metallicMap;
}

GLuint PBRMaterial::roughnessMap() const {
    return _roughnessMap;
}

GLuint PBRMaterial::normalMap() const {
    return _normalMap;
}

GLuint PBRMaterial::irradianceMap() const {
    return _irradianceMap;
}

GLuint PBRMaterial::prefilterMap() const {
    return _prefilterMap;
}

GLuint PBRMaterial::brdfLUT() const {
    return _brdfLUT;
}