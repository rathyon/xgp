#version 430

in vec3 texCoords;

uniform samplerCube skybox;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;

/* ==============================================================================
        Imports
 ============================================================================== */

vec3 toLinearRGB(vec3 color, float gamma);
vec3 toInverseGamma(vec3 color, float gamma);
vec3 reinhardToneMap(vec3 color);
vec3 exposureToneMap(vec3 color, float exposure);

out vec4 outColor;

void main() {
	vec3 envColor = reinhardToneMap(texture(skybox, texCoords).rgb);
	envColor = toInverseGamma(envColor, 2.2);
	outColor = vec4(envColor, 1.0);
}