#version 430

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
	Light light;
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

out vec4 outColor;

void main() {
	/**/
	vec3 V = normalize(vsIn.viewPosTS - vsIn.positionTS);
	vec3 N = normalize(texture(normalMap, vsIn.texCoords).rgb * 2.0 - 1.0);
	vec3 L = normalize(-vsIn.lightDirTS);
	vec3 H = normalize(L + V);

	float NdotL = max(dot(N, L), 0.0);
	float NdotH = max(dot(N, H), 0.0);

	vec3 diff = vec3(0.0);
	vec3 spec = vec3(0.0);

	vec3 testDiff = texture(diffuseMap, vsIn.texCoords).rgb;
	vec3 testSpec = vec3(1.0);

	if (NdotL > 0.0){
		diff = light.emission * ( testDiff * NdotL);
		spec = light.emission * ( testSpec * pow(NdotH, 64));
	}

	//outColor = vec4(texture(normalMap, vsIn.texCoords), 1.0);
	outColor = vec4(diff + spec, 1.0);
	/**/
}