#include <Renderer.h>

#include <Camera.h>
#include <Light.h>
#include <Shape.h>
#include <Scene.h>
#include <Skybox.h>

using namespace xgp;

Renderer::Renderer() : _gamma(2.2f), _exposure(3.0f), _drawSkybox(true) { }

void Renderer::setGamma(float gamma) {
    _gamma = gamma;
}

float Renderer::gamma() const {
    return _gamma;
}

float Renderer::exposure() const {
    return _exposure;
}

void Renderer::setExposure(float exp) {
    _exposure = exp;
}

void Renderer::setSkyboxDraw(bool state) {
    _drawSkybox = state;
}

void Renderer::uploadLightsBuffer(const Scene& scene) {
    glBindBuffer(GL_UNIFORM_BUFFER, _lightsBuffer);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        LightData data;
        scene.lights()[i]->toData(data);
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(LightData), sizeof(LightData), &data);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::uploadCameraBuffer(const Camera& camera) {
    CameraData data;
    data.viewMatrix = camera.viewMatrix();
    data.projMatrix = camera.projMatrix();
    data.viewProjMatrix = camera.viewProjMatrix();
    data.viewPos = camera.position();

    glBindBuffer(GL_UNIFORM_BUFFER, _cameraBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), &data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::uploadRendererBuffer() {
    RendererBuffer data;
    data.gamma = _gamma;
    data.exp   = _exposure;

    //TODO
}

void Renderer::drawShapes(const Scene& scene) {
    for (std::shared_ptr<Shape> shape : scene.shapes()) {
        shape->draw();
    }
}

void Renderer::drawSkybox(const Scene& scene) {
    if (scene.hasSkybox()) {
        const Skybox& sky = scene.skybox();
        sky.draw();
    }
}

void Renderer::prepare() {
    // Create shared buffers in the GPU and bind indices
    glGenBuffers(1, &_cameraBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, _cameraBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), 0, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_BUFFER_IDX, _cameraBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &_lightsBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, _lightsBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightData) * NUM_LIGHTS, 0, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, LIGHTS_BUFFER_IDX, _lightsBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::render(const Scene& scene, const Camera& camera) {
    // Upload constant buffers to the GPU
    //uploadRendererBuffer();
    uploadLightsBuffer(scene);
    uploadCameraBuffer(camera);

    drawShapes(scene);

    if (_drawSkybox)
        drawSkybox(scene);
}

