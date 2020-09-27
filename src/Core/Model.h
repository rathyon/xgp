#ifndef __XGP_MODEL_H__
#define __XGP_MODEL_H__

#include "Shape.h"
#include <vector>

namespace xgp {

    class Model : public Shape {
    public:
        Model(const std::shared_ptr<Geometry>& geo);
        Model(const std::shared_ptr<Geometry>& geo, const glm::vec3& position);
        Model(const std::shared_ptr<Geometry>& geo, const glm::mat4& objToWorld);

        void prepare() override;
        void draw()    override;
    };

    // OBJ file
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

    // loads models and materials from OBJ file
    std::vector<std::shared_ptr<Model>> loadOBJ(const std::string& filePath);
}

#endif