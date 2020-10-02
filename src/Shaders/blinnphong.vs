#version 430

#define LIGHT_COUNT 1

/* ==============================================================================
        Stage Inputs
 ============================================================================== */

layout(location = 0) in vec3 Position;	
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec3 Tangent;

/* ==============================================================================
        Structures
 ============================================================================== */

struct Light {
    vec3 position;
    vec3 direction;
    vec3 emission;
    float linear;
    float quadratic;
    int type;
    bool state;
    float cutoff;
};

/* ==============================================================================
        Uniforms
 ============================================================================== */

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform cameraBlock {
    mat4 ViewMatrix;
    mat4 ProjMatrix;
    mat4 ViewProjMatrix;
    vec3 ViewPos;
};

layout (std140) uniform lightBlock {
    Light light[LIGHT_COUNT];
};

/* ==============================================================================
        Stage Outputs
============================================================================== */
out FragData {
    vec3 position;
    vec3 normal; 
    vec2 texCoords;
    mat3 TBN;
} vsOut;

void main() {
    vsOut.position  = vec3(ModelMatrix * vec4(Position, 1.0));   
    vsOut.normal    = normalize(NormalMatrix * Normal);
    vsOut.texCoords = TexCoords;

    vec3 T = normalize(vec3(ModelMatrix * vec4(Tangent, 0.0)));
    vec3 N = normalize(vec3(ModelMatrix * vec4(Normal, 0.0)));
    T = normalize(T - dot(T,N) * N);
    vec3 B = cross(N,T);

    vsOut.TBN = mat3(T, B, N);

    gl_Position = ViewProjMatrix * vec4(vsOut.position, 1.0);
}