#version 430

/* ==============================================================================
        Stage Inputs
 ============================================================================== */
in FragData {
    vec3 position;
    vec3 normal; 
    vec2 texCoords;
    mat3 TBN;
} vsIn;

/* ==============================================================================
        Uniforms
 ============================================================================== */
uniform cameraBlock {
    mat4 ViewMatrix;
    mat4 ProjMatrix;
    mat4 ViewProjMatrix;
    vec3 ViewPos;
};

// Material parameters
uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float f0;

/* ==============================================================================
        Imports
 ============================================================================== */

vec3 toLinearRGB(vec3 color, float gamma);

/* ==============================================================================
        Stage Outputs
 ============================================================================== */
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMetalRough;

vec3 fetchAlbedo() {
	if(albedo.r >= 0.0)
		return albedo;
	else
		return toLinearRGB(texture(albedoMap, vsIn.texCoords).rgb, 2.2);
}

float fetchAlpha() {
	return texture(albedoMap, vsIn.texCoords).a;
}

float fetchParameter(sampler2D samp, float val) {
	if(val >= 0.0)
		return val;
	else
		return texture(samp, vsIn.texCoords).r;
}

vec3 fetchNormal() {
	vec3 normal = texture(normalMap, vsIn.texCoords).xyz;
	normal = normal * 2.0 - 1.0; // remap from [0,1] to [-1,1]
	return normalize(vsIn.TBN * normal); //from tangent space to world space
}

void main() {
	float metal = fetchParameter(metallicMap, metallic);
    float rough = fetchParameter(roughnessMap, roughness);

	gPosition = vsIn.position;
	gNormal = fetchNormal();
	//gAlbedo = vec4(fetchAlbedo(), fetchAlpha());
	gAlbedo = fetchAlbedo();
	gMetalRough = vec3(metal, rough, f0);
}