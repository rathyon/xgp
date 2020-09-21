#version 430

in vec3 texCoords;

uniform samplerCube skybox;

out vec4 outColor;

void main() {
	outColor = texture(skybox, texCoords);
}