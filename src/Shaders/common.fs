#version 430

// Math constants
const float PI = 3.14159265358979;

/* ==============================================================================
        Utils
 ============================================================================== */

vec3 toLinearRGB(vec3 color, float gamma) {
	return pow(color, vec3(gamma));
}

vec3 toInverseGamma(vec3 color, float gamma) {
	return pow(color, vec3(1.0/gamma));
}

vec3 reinhardToneMap(vec3 color) {
	return color / (color + vec3(1.0));
}

vec3 exposureToneMap(vec3 color, float exposure) {
	return vec3(1.0) - exp(-color * exposure);
}

vec2 parallaxMapping(sampler2D heightMap, vec2 uv, vec3 V, float heightScale) {
	float height = 1.0 - texture(heightMap, uv).r;
	//vec2 p = V.xy / V.z * (height * heightScale);
	vec2 p = V.xy * (height * heightScale);
	return uv - p;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

/* ============================================================================
        Diffuse BRDFs
 ============================================================================ */

 float brdfLambert() {
 	// return 1.0 / PI;
 	return 0.318309886183791;
 }

 /* ============================================================================
        Specular BRDFs
 ============================================================================*/

 vec3 specCookTorrance(float NdotV, float NdotL, float D, vec3 F, float G) {
 	vec3 num = D * F * G;
 	float denom = (4 * NdotV * NdotL); 
 	return num / max(denom, 0.001); // 0.001 to prevent divide by zero.
 }

 /* ============================================================================
        Microfacet Models
 ============================================================================*/
 // Normal Distribution Functions (NDFs)

float distBeckmann(float NdotH, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;

	float NdotH2 = NdotH * NdotH;

	float num = exp((NdotH2 - 1.0) / a2 * NdotH2);
	float denom = PI * a2 * (NdotH2 * NdotH2);
	return num / denom;

}

float distGGX(float NdotH, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;

	float NdotH2 = NdotH * NdotH;

	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * (denom * denom);
	return a2 / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

// Geometry Functions
float geoSchlickGGX(float NdotV, float roughness) {
	float k = (roughness + 1.0);
	k = (k * k) / 8.0;

	float denom = NdotV * (1.0 - k) + k;
	return NdotV / denom;
}

float geoSchlickGGX_IBL(float NdotV, float roughness) {
	float k = (roughness * roughness) / 2.0;

	float denom = NdotV * (1.0 - k) + k;
	return NdotV / denom;
}

float geoSmith(float NdotV, float NdotL, float roughness) {
	return geoSchlickGGX(NdotV, roughness) * geoSchlickGGX(NdotL, roughness);
}