#version 460 core
in vec3 _vertColor;

out vec4 FragColor;

void main() {
	FragColor = vec4(_vertColor, 1.0);
}