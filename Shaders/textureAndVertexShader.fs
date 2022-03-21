#version 330 core

in vec3 vertexColor;
in vec2 textureCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, textureCoord);
}