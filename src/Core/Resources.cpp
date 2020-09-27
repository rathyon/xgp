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

void Resources::addTexture(const std::string& name, const std::shared_ptr<Texture>& texture) {
    _textures[name] = texture;
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

bool Resources::deleteTexture(const std::string& name) {
    auto it = _textures.find(name);
    if (it != _textures.end()) {
        _textures.erase(name);
        return true;
    }

    return false;
}

Shader* Resources::getShader(const std::string& name) {
    if (_shaders.find(name) == _shaders.end())
        return nullptr;
    else
        return _shaders.at(name).get();
}

Geometry* Resources::getGeometry(const std::string& name) {
    if (_geometry.find(name) == _geometry.end())
        return nullptr;
    else
        return _geometry.at(name).get();
}

Shape* Resources::getShape(const std::string& name) {
    if (_shapes.find(name) == _shapes.end())
        return nullptr;
    else
        return _shapes.at(name).get();
}

Texture* Resources::getTexture(const std::string& name) {
    if (_textures.find(name) == _textures.end())
        return nullptr;
    else
        return _textures.at(name).get();
}

void Resources::cleanup() {
    _geometry.clear();
    _shapes.clear();
    _shaders.clear();
    _textures.clear();
}