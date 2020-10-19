#include <Renderer.h>

#include <Camera.h>
#include <Light.h>
#include <Shape.h>
#include <Scene.h>
#include <Skybox.h>
#include <Resources.h>

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

void Renderer::prepare(int width, int height, GLuint deferredProg) {
	_deferredProg = deferredProg;

	glGenFramebuffers(1, &_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);

	glGenTextures(1, &_gPosition);
	glBindTexture(GL_TEXTURE_2D, _gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gPosition, 0);

	glGenTextures(1, &_gNormal);
	glBindTexture(GL_TEXTURE_2D, _gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _gNormal, 0);

	glGenTextures(1, &_gAlbedo);
	glBindTexture(GL_TEXTURE_2D, _gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _gAlbedo, 0);

	glGenTextures(1, &_gMetalRough);
	glBindTexture(GL_TEXTURE_2D, _gMetalRough);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, _gMetalRough, 0);

	unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	unsigned int _gDepth;
	glGenRenderbuffers(1, &_gDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, _gDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _gDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Screen Quad Setup
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	glGenVertexArrays(1, &_quadVAO);
	GLuint quadVBO;
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(AttribType::POSITION);
	glVertexAttribPointer(AttribType::POSITION, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(AttribType::UV);
	glVertexAttribPointer(AttribType::UV, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

	// Forward rendering:
	/** /
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawShapes(scene);
	/**/

	// Deferred rendering:
	/**/
	glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawShapes(scene);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(_deferredProg);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _gPosition);
	glUniform1i(glGetUniformLocation(_deferredProg, "gPosition"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _gNormal);
	glUniform1i(glGetUniformLocation(_deferredProg, "gNormal"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _gAlbedo);
	glUniform1i(glGetUniformLocation(_deferredProg, "gAlbedo"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _gMetalRough);
	glUniform1i(glGetUniformLocation(_deferredProg, "gMetalRough"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, scene.skybox().irradianceMap());
	glUniform1i(glGetUniformLocation(_deferredProg, "irradianceMap"), 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, scene.skybox().ggxPrefilterMap());
	glUniform1i(glGetUniformLocation(_deferredProg, "prefilterMap"), 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, Resource.getImage("brdfLUT")->id());
	glUniform1i(glGetUniformLocation(_deferredProg, "brdfLUT"), 6);

	glBindVertexArray(_quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glUseProgram(0);
	/**/

    if (_drawSkybox)
        drawSkybox(scene);
}

void Renderer::reshape(int width, int height) {
	glBindTexture(GL_TEXTURE_2D, _gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, _gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, _gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, _gMetalRough);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
}