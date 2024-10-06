#version 460 core
layout (location = 0) in vec3 _vertexPos;
layout (location = 1) in vec3 _vertexColorInput;
layout (location = 2) in vec2 _vertexTexCoordInput;

out vec3 _vertColor;
out vec2 _texCoord;

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _projection;

void main() {
	gl_Position = _projection * _view * _model * vec4(_vertexPos, 1.0);
	_vertColor = _vertexColorInput;
	_texCoord = _vertexTexCoordInput;
}