#include "Model.h"

#include <iostream>

using namespace xgp;

Model::Model(const std::string& objPath) {
    _geometry = std::make_shared<Geometry>();

    // Load obj file
    if (!_geometry->loadObj(objPath)) {
        std::cerr << "ERROR: Failed to load OBJ file: " << objPath << std::endl;
    }

    //TODO: load material from file (if possible)

    //TODO: Register geometry in the resource manager
}

Model::Model(const std::string& objPath, const glm::vec3& position)
    : Shape(position) {
    _geometry = std::make_shared<Geometry>();

    // Load Obj file
    if (!_geometry->loadObj(objPath)) {
        std::cerr << "ERROR: Failed to load OBJ file: " << objPath << std::endl;
    }

    //TODO: load material from file (if possible)

    //TODO: Register geometry in the resource manager
}

Model::Model(const std::string& objPath, const glm::mat4& objToWorld)
    : Shape(objToWorld) {
    _geometry = std::make_shared<Geometry>();

    // Load Obj file
    if (!_geometry->loadObj(objPath)) {
        std::cerr << "ERROR: Failed to load OBJ file: " << objPath << std::endl;
    }

    //TODO: load material from file (if possible)

    //TODO: Register geometry in the resource manager
}

void Model::prepare() {
    // Upload geometry to the GPU
}

void Model::draw() {
    updateMatrix();

    glUseProgram(_material->program());

    glUniform4fv(glGetUniformLocation(_material->program(), "ModelMatrix"), 1, glm::value_ptr(objToWorld()));
    glUniform3fv(glGetUniformLocation(_material->program(), "NormalMatrix"), 1, glm::value_ptr(normalMatrix()));

    if (_material)
        _material->uploadData();

    // TODO:
    // draw geometry

    glUseProgram(0);
}