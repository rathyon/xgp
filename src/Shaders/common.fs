#version 430

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