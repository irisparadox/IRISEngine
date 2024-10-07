#version 460 core
in vec2 _texCoord;

out vec4 FragColor;

uniform sampler2D _Texture;


void main() {
	FragColor = texture(_Texture, _texCoord);
}