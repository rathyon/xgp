#version 430

#define LIGHT_COUNT 1

// Everything in world coordinates
in FragData {
    vec3 position;
    vec3 normal; 
    vec2 texCoords;
    vec3 positionTS;
    vec3 viewPosTS;
    vec3 lightPosTS;
    vec3 lightDirTS;
} vsIn;

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

out vec4 outColor;

// diffuse/albedo textures are usually stored as sRGB
vec3 toLinearRGB(vec3 color, float gamma) {
	return pow(color, vec3(gamma));
}

vec3 toInverseGamma(vec3 color, float gamma) {
	return pow(color, vec3(1.0/gamma));
}


float heightScale = 0.05f;
vec2 parallaxMapping(vec2 uv, vec3 V) {
	float height = 1.0 - texture(heightMap, uv).r;
	//vec2 p = V.xy / V.z * (height * heightScale);
	vec2 p = V.xy * (height * heightScale);
	return uv - p;
}

void main() {
	/**/
	vec3 V = normalize(vsIn.viewPosTS - vsIn.positionTS);

	//vec2 texCoords = parallaxMapping(vsIn.texCoords, V);
	vec2 texCoords = vsIn.texCoords;

	vec3 N = normalize(texture(normalMap, texCoords).rgb * 2.0 - 1.0);
	vec3 L = normalize(-vsIn.lightDirTS);
	vec3 H = normalize(L + V);
	vec3 I = normalize(vsIn.position - ViewPos);
	vec3 R = reflect(I, N);

	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);

	vec3 diff = vec3(0.0);
	vec3 spec = vec3(0.0);

	vec3 testDiff = toLinearRGB(texture(diffuseMap, texCoords).rgb, 2.2);
	vec3 testSpec = vec3(1.0);

	if (NdotL > 0.0){
		diff = light[0].emission * ( testDiff * NdotL);
		spec = light[0].emission * ( testSpec * pow(NdotH, 64));
	}

	outColor = vec4(toInverseGamma(diff + spec, 2.2), 1.0);
}