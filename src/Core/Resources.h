#ifndef __XGP_RESOURCES_H__
#define __XGP_RESOURCES_H__

#include <memory>
#include <unordered_map>

// Macro to syntax sugar the singleton getter
// ex: Resource.getTexture("albedo");
#define Resource Resources::get()

namespace xgp {

    class Shape;
    class Geometry;
    class Shader;
    class Texture;

    class Resources {
    public:
        ~Resources();

        static Resources& get();

        void initialize();

        void addGeometry (const std::string& name, const std::shared_ptr<Geometry>& geometry);
        void addShape    (const std::string& name, const std::shared_ptr<Shape>& shape);
        void addShader   (const std::string& name, const std::shared_ptr<Shader>& shader);
        void addTexture  (const std::string& name, const std::shared_ptr<Texture>& texture);

        bool deleteGeometry (const std::string& name);
        bool deleteShape    (const std::string& name);
        bool deleteShader   (const std::string& name);
        bool deleteTexture  (const std::string& name);

        Geometry* getGeometry (const std::string& name);
        Shape*    getShape    (const std::string& name);
        Shader*   getShader   (const std::string& name);
        Texture*  getTexture  (const std::string& name);

        void cleanup();

    private:
        Resources() {}

        // To make sure Singleton copies don't appear
        Resources(Resources const&);      // Not implemented
        void operator=(Resources const&); // Not implemented

        std::unordered_map<std::string, std::shared_ptr<Geometry>> _geometry;
        std::unordered_map<std::string, std::shared_ptr<Shape>>    _shapes;
        std::unordered_map<std::string, std::shared_ptr<Shader>>   _shaders;
        std::unordered_map<std::string, std::shared_ptr<Texture>>  _textures;
    };

}

#endif