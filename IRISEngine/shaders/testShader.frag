#version 460 core
in vec3 _vertColor;
in vec2 _texCoord;

out vec4 FragColor;

uniform sampler2D _Texture;

void main() {
	FragColor = texture(_Texture, _texCoord) * vec4(_vertColor, 1.0);
}