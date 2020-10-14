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
        Structures
 ============================================================================== */
struct Light {
	vec3 position;
	vec3 direction;
	vec3 emission;
	int type;
	bool state;
	float cutoff;
};

/* ==============================================================================
        Uniforms
 ============================================================================== */
uniform cameraBlock {
    mat4 ViewMatrix;
    mat4 ProjMatrix;
    mat4 ViewProjMatrix;
    vec3 ViewPos;
};

const int LIGHT_COUNT = 1;

layout (std140) uniform lightBlock {
	Light lights[LIGHT_COUNT];
};

// Material parameters
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform samplerCube envMap;

/* ==============================================================================
        Imports
 ============================================================================== */

vec3 toLinearRGB(vec3 color, float gamma);
vec3 toInverseGamma(vec3 color, float gamma);
vec3 reinhardToneMap(vec3 color);

/* ==============================================================================
        Stage Outputs
 ============================================================================== */
out vec4 outColor;

vec3 fetchDiffuse() {
	if(diffuse.r >= 0.0)
		return diffuse;
	else
		return toLinearRGB(texture(diffuseMap, vsIn.texCoords).rgb, 2.2);
}

vec3 fetchSpecular() {
	if(specular.r >= 0.0)
		return specular;
	else
		return texture(specularMap, vsIn.texCoords).rgb;
}

vec3 fetchNormal() {
	vec3 normal = texture(normalMap, vsIn.texCoords).rgb;
	normal = normal * 2.0 - 1.0; // remap from [0,1] to [-1,1]
	return normalize(vsIn.TBN * normal); //from tangent space to world space
}

float fetchAlpha() {
	return texture(diffuseMap, vsIn.texCoords).a;
}

void main() {
	vec3 V = normalize(ViewPos - vsIn.position);

	vec3 N = fetchNormal();
	vec3 L = normalize(-lights[0].direction);
	vec3 H = normalize(L + V);
	vec3 I = normalize(vsIn.position - ViewPos);
	vec3 R = reflect(I, N);

	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);

	vec3 diff = vec3(0.0);
	vec3 spec = vec3(0.0);

	if (NdotL > 0.0){
		diff = lights[0].emission * ( fetchDiffuse() * NdotL);
		spec = lights[0].emission * ( fetchSpecular() * pow(NdotH, 64));
	}

	outColor = vec4(toInverseGamma(reinhardToneMap(diff + spec), 2.2), fetchAlpha());
}