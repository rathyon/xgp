#ifndef __XGP_GEOMETRY_H__
#define __XGP_GEOMETRY_H__

#include <GL/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>
#include <string>

namespace xgp {

    const enum AttribType {
        POSITION = 0,
        NORMAL   = 1,
        UV       = 2,
        TANGENT  = 3,
    };

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
        const std::vector<GLuint>& indices()  const;
        const GLuint VAO() const;

        void computeTangents();

        bool loadObj(const std::string& filePath);

        void upload();

    private:
        GLuint _vao;
        std::vector<GLuint> _indices;
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