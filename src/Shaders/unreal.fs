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

const int LIGHT_COUNT = 1;

layout (std140) uniform lightBlock {
	Light lights[LIGHT_COUNT];
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

vec3 fetchAlbedo() {
	if(albedo.r >= 0.0)
		return albedo;
	else
		return toLinearRGB(texture(albedoMap, vsIn.texCoords).rgb, 2.2);
}

float fetchParameter(sampler2D samp, float val) {
	if(val >= 0.0)
		return val;
	else
		return texture(samp, vsIn.texCoords).r;
}

vec3 fetchNormal() {
	vec3 normal = texture(normalMap, vsIn.texCoords).rgb;
	normal = normal * 2.0 - 1.0; // remap from [0,1] to [-1,1]
	return normalize(vsIn.TBN * normal); //from tangent space to world space
}

float fetchAlpha() {
	return texture(albedoMap, vsIn.texCoords).a;
}

void main() {
	vec3 N = fetchNormal(); 
    vec3 V = normalize(ViewPos - vsIn.position);

    vec3 albedo = fetchAlbedo();
    float metal = fetchParameter(metallicMap, metallic);
    float rough = fetchParameter(roughnessMap, roughness);

    vec3 F0 = mix(vec3(f0), albedo, metal);

    float NdotV = max(dot(N, V), 0.0);
	/* ==============================================================================
            Direct Lighting
    ============================================================================== */
	vec3 direct = vec3(0.0);
	for(int i=0; i < LIGHT_COUNT; i++){
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
			L = normalize(lights[i].position - vsIn.position);
			float distance = length(lights[i].position - vsIn.position);
    		Li = lights[i].emission * (1.0 / (distance * distance)); // inverse square law
		}
		// spot lights (for now they are the same as point lights)
		else{
			L = normalize(lights[i].position - vsIn.position);
			float distance = length(lights[i].position - vsIn.position);
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

	/* ==============================================================================
            Indirect/Ambient Lighting (IBL)
    ============================================================================== */
    // Diffuse component
    vec3 irradiance = texture(irradianceMap, N).rgb;

    vec3 kS = fresnelSchlickRoughness(NdotV, F0, rough);
	vec3 kD = 1.0 - kS;

	vec3 diffuse = irradiance * albedo;

	// Specular component
	vec3 R = reflect(-V, N);
	vec3 pref = textureLod(prefilterMap, R, rough * MAX_REFLECTION_LOD).rgb;
	vec2 brdf = texture(brdfLUT, vec2(NdotV, rough)).rg;

	vec3 specular = pref * (kS * brdf.x + brdf.y);

	vec3 ambient = kD * diffuse + specular;
	/* ==============================================================================
            Post-processing
    ============================================================================== */ 
	vec3 color = reinhardToneMap(direct + ambient);
	color = toInverseGamma(color, 2.2);
	outColor = vec4(color, fetchAlpha());
}