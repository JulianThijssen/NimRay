#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 pass_texCoords;

void main(void) {
	pass_texCoords = texCoords;

	gl_Position = vec4(position, 1);
}
