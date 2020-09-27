#include "Geometry.h"

#include <iostream>
#include <unordered_map>

using namespace xgp;

Geometry::Geometry() { }

const GLuint Geometry::VAO() const {
    return _vao;
}

const std::vector<Vertex>& Geometry::vertices() const {
    return _vertices;
}

const std::vector<GLuint>& Geometry::indices() const {
    return _indices;
}

void Geometry::setVertices(const std::vector<Vertex>& vertices) {
    _vertices.resize(vertices.size());
    std::copy(vertices.begin(), vertices.end(), _vertices.begin());
}

void Geometry::setIndices(const std::vector<GLuint>& indices) {
    _indices.resize(indices.size());
    std::copy(indices.begin(), indices.end(), _indices.begin());
}

void Geometry::computeTangents() {
    // for each triangle, e.g every 3 vertices in _indices
    for (int i = 0; i < _indices.size(); i += 3) {

        // position
        glm::vec3& v0 = _vertices[_indices[i + 0]].pos;
        glm::vec3& v1 = _vertices[_indices[i + 1]].pos;
        glm::vec3& v2 = _vertices[_indices[i + 2]].pos;

        // uv
        glm::vec2& uv0 = _vertices[_indices[i + 0]].texCoord;
        glm::vec2& uv1 = _vertices[_indices[i + 1]].texCoord;
        glm::vec2& uv2 = _vertices[_indices[i + 2]].texCoord;

        // edges
        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        // determinant
        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

        _vertices[_indices[i + 0]].tangent += tangent;
        _vertices[_indices[i + 1]].tangent += tangent;
        _vertices[_indices[i + 2]].tangent += tangent;
    }

    // an alternative would be to not normalize, "weighing" the tangents by triangle size
    for (int i = 0; i < _vertices.size(); i++) {
        _vertices[i].tangent = glm::normalize(_vertices[i].tangent);
    }
}

void Geometry::upload() {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // 2 VBOs per VAO: vertices + indices
    GLuint VBOs[2] = { 0, 0 };
    glGenBuffers(2, VBOs);

    // vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

    // vertex buffer attribute layout
    glEnableVertexAttribArray(AttribType::POSITION);
    glVertexAttribPointer(AttribType::POSITION, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), (const void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(AttribType::NORMAL);
    glVertexAttribPointer(AttribType::NORMAL, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), (const void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(AttribType::UV);
    glVertexAttribPointer(AttribType::UV, 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(AttribType::TANGENT);
    glVertexAttribPointer(AttribType::TANGENT, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(Vertex), (const void*)offsetof(Vertex, tangent));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

    // They MUST be placed in this order. Why? I couldn't figure out, seems to be an issue though
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}