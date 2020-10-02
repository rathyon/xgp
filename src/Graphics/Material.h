#ifndef __XGP_MATERIAL_H__
#define __XGP_MATERIAL_H__

#include <GL/glew.h>
#include <string>

#include <Skybox.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace xgp {

    class Material {
    public:
        Material() { }

        GLuint program() const;

        void setProgram(GLuint prog);

        virtual void uploadData() = 0;

        virtual void update(const Skybox& skybox) = 0;

    protected:
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void setVec3(const std::string& name, glm::vec3& vec);
        void setVec4(const std::string& name, glm::vec4& vec);
        void setMat3(const std::string& name, glm::mat3& mat);
        void setMat4(const std::string& name, glm::mat4& mat);
        void setSampler(const std::string& name, int id);

        GLuint _program;
    };

}

#endif