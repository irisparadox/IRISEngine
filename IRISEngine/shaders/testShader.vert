#version 460 core
layout (location = 0) in vec3 _vertexPos;
layout (location = 1) in vec3 _vertexColorInput;
layout (location = 2) in vec2 _vertexTexCoordInput;

out vec3 _vertColor;
out vec2 _texCoord;

void main() {
	gl_Position = vec4(_vertexPos, 1.0);
	_vertColor = _vertexColorInput;
	_texCoord = _vertexTexCoordInput;
}