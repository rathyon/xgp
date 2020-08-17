#ifndef __XGP_SCENE_H__
#define __XGP_SCENE_H__

#include <Shape.h>
#include <Light.h>

#include <vector>

namespace xgp {

    class Scene {
    public:
        Scene();

        void addShape (const std::shared_ptr<Shape>&  shape);      
        void addLight (const std::shared_ptr<Light>&  light);

        const std::vector<std::shared_ptr<Shape>>&  shapes()  const;
        const std::vector<std::shared_ptr<Light>>&  lights()  const;


    private:
        std::vector<std::shared_ptr<Shape>>  _shapes;
        std::vector<std::shared_ptr<Light>>  _lights;
    };

}

#endif