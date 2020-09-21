#include "Shape.h"

using namespace xgp;

Shape::Shape() { }
Shape::Shape(const glm::vec3& position) : SceneObject(position){ }
Shape::Shape(const glm::mat4& objToWorld) : SceneObject(objToWorld) { }

const std::shared_ptr<Geometry>& Shape::geometry() const {
    return _geometry;
}

const std::shared_ptr<Material>& Shape::material() const {
    return _material;
}

void Shape::updateMatrix() {
    SceneObject::updateMatrix();
    _normalMatrix = glm::transpose(glm::inverse(glm::mat3(_objToWorld)));
}

const glm::mat3& Shape::normalMatrix() const {
    return _normalMatrix;
}

void Shape::setMaterial(const std::shared_ptr<Material>& mat) {
    _material = mat;
}

void Shape::updateMaterial(const Skybox& skybox) {
    _material->update(skybox);
}