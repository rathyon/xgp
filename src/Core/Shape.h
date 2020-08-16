#ifndef __XGP_SHAPE_H__
#define __XGP_SHAPE_H__

#include "SceneObject.h"
#include "Geometry.h"
#include "Material.h"

namespace xgp {

    class Shape : public SceneObject {
    public:
        Shape();
        Shape(const glm::vec3& position);
        Shape(const glm::mat4& objToWorld);

        virtual void prepare() = 0;
        virtual void draw() = 0;

        const std::shared_ptr<Material>& material() const;
        const std::shared_ptr<Geometry>& geometry() const;

        const glm::mat3& normalMatrix() const;

        void updateMatrix() override;

        void setMaterial(const std::shared_ptr<Material>& mat);

    protected:
        std::shared_ptr<Geometry> _geometry;
        std::shared_ptr<Material> _material;

        glm::mat3 _normalMatrix;
    };

}

#endif