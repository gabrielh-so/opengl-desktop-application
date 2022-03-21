#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 thisColour;
layout (location = 2) in vec2 aTextureCoord;

out vec3 vertexColor;
out vec2 textureCoord;

void main()
{
   gl_Position = vec4(aPos, 1.0);
   vertexColor = thisColour;
   textureCoord = aTextureCoord;
}