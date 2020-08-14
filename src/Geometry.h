#ifndef __XGP_GEOMETRY_H__
#define __XGP_GEOMETRY_H__

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>
#include <string>

namespace xgp {

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoord;
        glm::vec3 tangent;
    };

    class Geometry {
    public:
        Geometry();

        const std::vector<Vertex>& vertices() const;
        const std::vector<uint32_t>& indices()  const;

        void computeTangents();

        bool loadObj(const std::string& filePath);

    private:
        std::vector<uint32_t> _indices;
        std::vector<Vertex> _vertices;
    };

    struct ObjVertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoord;

        bool operator==(const ObjVertex& v) const {
            return pos == v.pos &&
                   normal == v.normal &&
                   texCoord == v.texCoord;
        }
    };

}

#endif