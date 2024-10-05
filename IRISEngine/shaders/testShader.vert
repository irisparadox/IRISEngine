#version 460 core
layout (location = 0) in vec3 _vertexPos;
layout (location = 1) in vec3 _vertexColorInput;

out vec3 _vertColor;

void main() {
	gl_Position = vec4(_vertexPos, 1.0);
	_vertColor = _vertexColorInput;
}