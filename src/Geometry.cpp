#include "Geometry.h"

#include <iostream>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using namespace xgp;

namespace std {
    template<> struct hash<ObjVertex> {
        size_t operator()(ObjVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

Geometry::Geometry() { }

const std::vector<Vertex>& Geometry::vertices() const {
    return _vertices;
}

const std::vector<uint32_t>& Geometry::indices() const {
    return _indices;
}

bool Geometry::loadObj(const std::string& filePath) {
    tinyobj::ObjReader objReader = tinyobj::ObjReader();
    tinyobj::ObjReaderConfig config = tinyobj::ObjReaderConfig();
    config.vertex_color = false;

    if(!objReader.ParseFromFile(filePath, config)) {
        std::cerr << objReader.Error() << std::endl;
        return false;
    }
    std::cout << objReader.Warning() << std::endl;

    tinyobj::attrib_t attrib = objReader.GetAttrib();

    std::unordered_map<ObjVertex, unsigned int> uniqueVertices{};
    for (const auto& shape : objReader.GetShapes()) {
        for (const auto& index : shape.mesh.indices) {
            ObjVertex objVertex{};

            objVertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            objVertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            objVertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
                //1.0f - attrib.texcoords[2 * index.texcoord_index + 1] in case it needs to be flipped
            };

            if (uniqueVertices.count(objVertex) == 0) {
                uniqueVertices[objVertex] = static_cast<uint32_t>(_vertices.size());

                // includes (empty) tangent attribute
                Vertex vertex{};
                vertex.pos = objVertex.pos;
                vertex.normal = objVertex.normal;
                vertex.texCoord = objVertex.texCoord;
                vertex.tangent = glm::vec3(0);

                _vertices.push_back(vertex);
            }

            _indices.push_back(uniqueVertices[objVertex]);
        }
    }

    computeTangents();
    return true;
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