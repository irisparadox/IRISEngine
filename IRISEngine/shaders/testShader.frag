#version 460 core
in vec2 _texCoord;

out vec4 FragColor;

uniform sampler2D _Texture1;
uniform sampler2D _Texture2;


void main() {
	FragColor = mix(texture(_Texture1, _texCoord), texture(_Texture2, _texCoord), 0.2);
}