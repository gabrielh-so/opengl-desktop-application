#version 330 core

in vec2 textureCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float heatLevel;

void main()
{
    FragColor = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), heatLevel);
}