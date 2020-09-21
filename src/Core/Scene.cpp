#include <Scene.h>

#include <Shape.h>

using namespace xgp;

Scene::Scene() { }

void Scene::addShape(const std::shared_ptr<Shape>& shape) {
    _shapes.push_back(shape);
}

void Scene::addLight(const std::shared_ptr<Light>& light) {
    _lights.push_back(light);
}

void Scene::setSkybox(const Skybox& skybox) {
    _skybox = &skybox;

    for (std::shared_ptr<Shape> shape : _shapes) {
        shape->updateMaterial(skybox);
    }
}

const std::vector<std::shared_ptr<Shape>>& Scene::shapes() const {
    return _shapes;
}

const std::vector<std::shared_ptr<Light>>& Scene::lights() const {
    return _lights;
}

const Skybox& Scene::skybox() const {
    return *_skybox;
}

bool Scene::hasSkybox() const {
    return _skybox != nullptr;
}