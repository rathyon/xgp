#ifndef __XGP_MODEL_H__
#define __XGP_MODEL_H__

#include "Shape.h"

namespace xgp {

    class Model : public Shape {
    public:
        Model(const std::string& objPath);
        Model(const std::string& objPath, const glm::vec3& position);
        Model(const std::string& objPath, const glm::mat4& objToWorld);

        void prepare() override;
        void draw()    override;
    };
}

#endif