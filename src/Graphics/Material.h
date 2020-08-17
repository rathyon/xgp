#ifndef __XGP_MATERIAL_H__
#define __XGP_MATERIAL_H__

#include <GL/glew.h>
// temporary replacement for future Color class
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace xgp {

    enum TextureUnit {
        DIFFUSE_MAP  = 0,
        SPECULAR_MAP = 1,
        NORMAL_MAP   = 2,
    };

    class Material {
    public:
        Material() { }

        void use() const;
        GLuint program() const;

        //TODO: delet this when loading shader from resource manager
        void setProgram(GLuint prog);

        virtual void uploadData() = 0;

        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void setVec3(const std::string& name, glm::vec3& vec);
        void setVec4(const std::string& name, glm::vec4& vec);
        void setMat3(const std::string& name, glm::mat3& mat);
        void setMat4(const std::string& name, glm::mat4& mat);
        void setSampler(const std::string& name, int id);

    protected:
        GLuint _program;
    };

}

#endif