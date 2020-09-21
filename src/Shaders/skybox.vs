#version 430

layout(location = 0) in vec3 Position;

uniform cameraBlock {
    mat4 ViewMatrix;
    mat4 ProjMatrix;
    mat4 ViewProjMatrix;
    vec3 ViewPos;
};

out vec3 texCoords;

void main() {
	texCoords = Position;

	mat4 rotMatrix = mat4(mat3(ViewMatrix));
	vec4 clipPos = ProjMatrix * rotMatrix * vec4(Position, 1.0);

	gl_Position = clipPos.xyww;
}