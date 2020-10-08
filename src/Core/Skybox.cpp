#include <Skybox.h>

#include <Resources.h>
#include <Shader.h>
#include <Utils.h>
#include <Material.h>

using namespace xgp;

Skybox::Skybox(const std::string& env, const std::string& irradiance, const std::string& ggx) {
    _envMap = Cubemap(env, CubemapType::SKYBOX).id();
    _irradianceMap = Cubemap(irradiance, CubemapType::IRRADIANCE).id();
    _ggxPrefilterMap = Cubemap(ggx, CubemapType::GGX).id();

    genCubeGeometry();

    _prog = Resource.getShader("skybox")->id();
}

Skybox::~Skybox() { }

void Skybox::draw() const{
    glUseProgram(_prog);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _envMap);
    glUniform1i(glGetUniformLocation(_prog, "envMap"), 0);

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glUseProgram(0);
}

GLuint Skybox::envMap() const {
    return _envMap;
}

GLuint Skybox::irradianceMap() const {
    return _irradianceMap;
}

GLuint Skybox::ggxPrefilterMap() const {
    return _ggxPrefilterMap;
}

void Skybox::genCubeGeometry() {
    float skyboxVertices[] = {
        //back
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        //left
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        //right
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

         //front
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        //top
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        //bottom
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Utils::checkOpenGLError("Error generating skybox geometry!");
}