#version 430

layout(location = 0) in vec3 Position;	
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec3 Tangent;

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

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform cameraBlock {
    mat4 ViewMatrix;
    mat4 ProjMatrix;
    mat4 ViewProjMatrix;
    vec3 ViewPos;
};

layout (std140) uniform lightBlock {
    Light light;
};

// Passes everything in world coordinates to the fragment shader
out FragData {
    vec3 position;
    vec3 normal; 
    vec2 texCoords;
    vec3 positionTS;
    vec3 viewPosTS;
    vec3 lightPosTS;
    vec3 lightDirTS;
} vsOut;

void main() {
    // Everything in world coordinates
    vsOut.position  = vec3(ModelMatrix * vec4(Position, 1.0));   
    vsOut.normal    = normalize(NormalMatrix * Normal);
    vsOut.texCoords = TexCoords;

    vec3 T = normalize(vec3(ModelMatrix * vec4(Tangent, 0.0)));
    vec3 N = normalize(vec3(ModelMatrix * vec4(Normal, 0.0)));
    T = normalize(T - dot(T,N) * N);
    vec3 B = cross(N,T);

    mat3 TBN = transpose(mat3(T, B, N));

    vsOut.positionTS = TBN * vec3(ModelMatrix * vec4(Position, 1.0));
    vsOut.viewPosTS = TBN * ViewPos;
    vsOut.lightPosTS = TBN * light.position;
    vsOut.lightDirTS = TBN * light.direction;

    // Return position in MVP coordinates
    gl_Position = ViewProjMatrix * vec4(vsOut.position, 1.0);
}