#version 430

#define LIGHT_COUNT 1

/* ==============================================================================
        Stage Inputs
 ============================================================================== */
in FragData {
    vec3 position;
    vec3 normal; 
    vec2 texCoords;
    vec3 positionTS;
    vec3 viewPosTS;
    vec3 lightPosTS;
    vec3 lightDirTS;
} vsIn;

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
uniform cameraBlock {
    mat4 ViewMatrix;
    mat4 ProjMatrix;
    mat4 ViewProjMatrix;
    vec3 ViewPos;
};

layout (std140) uniform lightBlock {
	Light light[LIGHT_COUNT];
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
vec3 exposureToneMap(vec3 color, float exposure);

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
	return normalize(texture(normalMap, vsIn.texCoords).rgb * 2.0 - 1.0);
}

float fetchAlpha() {
	return texture(diffuseMap, vsIn.texCoords).a;
}

void main() {
	/** /
	vec3 V = normalize(vsIn.viewPosTS - vsIn.positionTS);

	vec3 N = fetchNormal();
	vec3 L = normalize(-vsIn.lightDirTS);
	vec3 H = normalize(L + V);
	vec3 I = normalize(vsIn.position - ViewPos);
	vec3 R = reflect(I, N);

	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);

	vec3 diff = vec3(0.0);
	vec3 spec = vec3(0.0);

	if (NdotL > 0.0){
		diff = light[0].emission * ( fetchDiffuse() * NdotL);
		spec = light[0].emission * ( fetchSpecular() * pow(NdotH, 64));
	}

	outColor = vec4(toInverseGamma(diff + spec, 2.2), 1.0);
	/**/
	outColor = vec4(fetchDiffuse(), fetchAlpha());
}