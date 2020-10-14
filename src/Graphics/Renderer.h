#ifndef __XGP_RENDERER_H__
#define __XGP_RENDERER_H__

#include <Scene.h>
#include <Camera.h>

namespace xgp {

    const unsigned int NUM_LIGHTS = 1;
    
    enum ToneOperator {
        SIMPLE,
        REINHART,
        UNCHARTED
    };

    enum BufferIndices {
        CAMERA_BUFFER_IDX   = 0,
        LIGHTS_BUFFER_IDX   = 1,
        RENDERER_BUFFER_IDX = 2
    };

    // Buffer for shaders with renderer information
    struct RendererBuffer {
        float gamma;
        float exp;
    };
    
    class Renderer {
    public:
        Renderer();

        void prepare();
        void render(const Scene& scene, const Camera& camera);

        float gamma() const;
        void setGamma(float gamma);

        float exposure() const;
        void setExposure(float exp);

        void setSkyboxDraw(bool state);

    private:
        void uploadRendererBuffer();
        void uploadLightsBuffer(const Scene& scene);
        void uploadCameraBuffer(const Camera& camera);
        void drawShapes(const Scene& scene);
        void drawSkybox(const Scene& scene);

        float _gamma;
        float _exposure;
        ToneOperator _tone;

        bool _drawSkybox;
        
        GLuint _lightsBuffer;
        GLuint _cameraBuffer;
        GLuint _rendererBuffer;
    };

}

#endif
