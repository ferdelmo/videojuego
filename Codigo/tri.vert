#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertex_color;

out vec3 color;

void main() {
	color = vertex_color;
	gl_Position = vec4(position, 1.0);
}