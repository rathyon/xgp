#version 430

/* ==============================================================================
        Stage Inputs
 ============================================================================== */
in vec2 texCoords;

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
layout (binding = 0) uniform cameraBlock {
    mat4 ViewMatrix;
    mat4 ProjMatrix;
    mat4 ViewProjMatrix;
    vec3 ViewPos;
};

const int NUM_LIGHTS = 1;

layout (std140, binding = 1) uniform lightBlock {
	Light lights[NUM_LIGHTS];
};

// GBuffer Maps
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMetalRough;

// IBL precomputation
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

const float MAX_REFLECTION_LOD = 4.0;

/* ==============================================================================
        Imports
 ============================================================================== */

vec3 toLinearRGB(vec3 color, float gamma);
vec3 toInverseGamma(vec3 color, float gamma);
vec3 reinhardToneMap(vec3 color);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float brdfLambert();
vec3 specCookTorrance(float NdotV, float NdotL, float D, vec3 F, float G);
float distGGX(float NdotH, float roughness);
float geoSchlickGGX(float NdotV, float roughness);
float geoSchlickGGX_IBL(float NdotV, float roughness);
float geoSmith(float NdotV, float NdotL, float roughness);

/* ==============================================================================
        Stage Outputs
 ============================================================================== */
out vec4 outColor;

void main() {
	vec3 Position = texture(gPosition, texCoords).rgb;
	vec3 N = texture(gNormal, texCoords).rgb;
    vec3 V = normalize(ViewPos - Position);

    vec3 albedo = texture(gAlbedo, texCoords).rgb;
    float metallic = texture(gMetalRough, texCoords).r;
    float rough = texture(gMetalRough, texCoords).g;
    float f0 = texture(gMetalRough, texCoords).b;

    vec3 F0 = mix(vec3(f0), albedo, metallic);

    float NdotV = max(dot(N, V), 0.0);

	/* ==============================================================================
            Direct Lighting
    ============================================================================== */
    /**/
	vec3 direct = vec3(0.0);
	for(int i=0; i < NUM_LIGHTS; i++){
		if(!lights[i].state){
			continue;
		}

		vec3 L;
		vec3 Li;

		// directional lights
		if(lights[i].type == 0){
			L = -lights[i].direction;
			Li = lights[i].emission;
		}
		// point lights
		else if(lights[i].type == 1){
			L = normalize(lights[i].position - Position);
			float distance = length(lights[i].position - Position);
    		Li = lights[i].emission * (1.0 / (distance * distance)); // inverse square law
		}
		// spot lights (for now they are the same as point lights)
		else{
			L = normalize(lights[i].position - Position);
			float distance = length(lights[i].position - Position);
    		Li = lights[i].emission * (1.0 / (distance * distance)); // inverse square law
		}

		vec3 H = normalize(V + L);
		float NdotH = max(dot(N, H), 0.0);
		float NdotL = max(dot(N, L), 0.0);
		float HdotV = max(dot(H, V), 0.0);	

    	// NDF Term
    	float D = distGGX(NdotH, rough);

    	// Fresnel Term
    	vec3 F = fresnelSchlick(HdotV, F0);

    	// Geometric Term
    	float G = geoSmith(NdotV, NdotL, rough);

    	vec3 kS = F;
    	vec3 kD = vec3(1.0) - kS;
    	kD = kD * (1.0 - metallic);

    	vec3 specular = specCookTorrance(NdotV, NdotL, D, F, G);
    	vec3 diffuse = kD * albedo * brdfLambert();

    	direct += (diffuse + specular) * Li * NdotL;
	}
	/**/
	/* ==============================================================================
            Indirect/Ambient Lighting (IBL)
    ============================================================================== */
    /** /
    // Diffuse component
    vec3 irradiance = texture(irradianceMap, N).rgb;

    vec3 kS = fresnelSchlickRoughness(NdotV, F0, rough);
	vec3 kD = 1.0 - kS;
	kD = kD * (1.0 - metallic);

	vec3 diffuse = irradiance * albedo;

	// Specular component
	vec3 R = reflect(-V, N);
	vec3 pref = textureLod(prefilterMap, R, rough * MAX_REFLECTION_LOD).rgb;
	vec2 brdf = texture(brdfLUT, vec2(NdotV, rough)).rg;

	vec3 specular = pref * (kS * brdf.x + brdf.y);

	vec3 ambient = (kD * diffuse) + specular;
	/**/
	/* ==============================================================================
            Post-processing
    ============================================================================== */
    /**/
	vec3 color = reinhardToneMap(direct);
	color = toInverseGamma(color, 2.2);
	//outColor = vec4(color, fetchAlpha());
	outColor = vec4(color, 1.0);
	/**/
}