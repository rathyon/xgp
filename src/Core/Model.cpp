#include "Model.h"

#include <iostream>

#include <Utils.h>

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
    //potentially other things...

    _geometry->upload();
}

void Model::draw() {
    updateMatrix();

    glUseProgram(_material->program());

    glUniformMatrix4fv(glGetUniformLocation(_material->program(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(objToWorld()));
    glUniformMatrix3fv(glGetUniformLocation(_material->program(), "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix()));

    if (_material)
        _material->uploadData();

    glBindVertexArray(_geometry->VAO());
    //glDrawArrays(GL_TRIANGLES, 0, _geometry->vertices().size());
    glDrawElements(GL_TRIANGLES, _geometry->indices().size(), GL_UNSIGNED_INT, 0); // has a specific "problem", check Geometry class for details
    glBindVertexArray(0);

    glUseProgram(0);
}