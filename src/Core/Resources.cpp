#include <Resources.h>

#include <Geometry.h>
#include <Shape.h>
#include <Shader.h>
#include <Image.h>

using namespace xgp;

Resources::~Resources() { }

Resources& Resources::get() {
    static Resources _instance; // Guaranteed to be destroyed.
    return _instance;           // Instantiated on first use.
}

void Resources::initialize() {
    // Instantiate Resource Manager
    get();
}

void Resources::addGeometry(const std::string& name, const std::shared_ptr<Geometry>& geometry) {
    _geometry[name] = geometry;
}

void Resources::addShape(const std::string& name, const std::shared_ptr<Shape>& shape) {
    _shapes[name] = shape;
}

void Resources::addShader(const std::string& name, const std::shared_ptr<Shader>& shader) {
    _shaders[name] = shader;
}

void Resources::addImage(const std::string& name, const std::shared_ptr<Image>& image) {
    _images[name] = image;
}

bool Resources::deleteGeometry(const std::string& name) {
    auto it = _geometry.find(name);
    if (it != _geometry.end()) {
        _geometry.erase(name);
        return true;
    }

    return false;
}

bool Resources::deleteShape(const std::string& name) {
    auto it = _shapes.find(name);
    if (it != _shapes.end()) {
        _shapes.erase(name);
        return true;
    }

    return false;
}

bool Resources::deleteShader(const std::string& name) {
    auto it = _shaders.find(name);
    if (it != _shaders.end()) {
        _shaders.erase(name);
        return true;
    }

    return false;
}

bool Resources::deleteImage(const std::string& name) {
    auto it = _images.find(name);
    if (it != _images.end()) {
        _images.erase(name);
        return true;
    }

    return false;
}

Shader* Resources::getShader(const std::string& name) {
    return _shaders.at(name).get();
}

Geometry* Resources::getGeometry(const std::string& name) {
    return _geometry.at(name).get();
}

Shape* Resources::getShape(const std::string& name) {
    return _shapes.at(name).get();
}

Image* Resources::getImage(const std::string& name) {
    return _images.at(name).get();
}

void Resources::cleanup() {
    _geometry.clear();
    _shapes.clear();
    _shaders.clear();
    _images.clear();
}