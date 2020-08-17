#version 430

// Everything in world coordinates
in FragData {
    vec3 position;
    vec3 normal; 
    vec2 texCoords;
} vsIn;

uniform cameraBlock {
    mat4 ViewMatrix;
    mat4 ProjMatrix;
    mat4 ViewProjMatrix;
    vec3 ViewPos;
};

// Material parameters
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

out vec4 outColor;

void main() {
	outColor = vec4(vsIn.normal, 1.0);
}