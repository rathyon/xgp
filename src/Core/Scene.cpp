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

const std::vector<std::shared_ptr<Shape>>& Scene::shapes() const {
    return _shapes;
}

const std::vector<std::shared_ptr<Light>>& Scene::lights() const {
    return _lights;
}