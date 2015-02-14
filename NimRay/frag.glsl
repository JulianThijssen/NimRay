#version 430 core

uniform sampler2D framebuffer;

in vec2 pass_texCoords;

out vec4 out_Color;

void main(void) {
	out_Color = texture(framebuffer, pass_texCoords);
}
