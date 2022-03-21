#version 330 core

out vec3 FragmentPosition;

layout (location = 0) in vec3 aPos;

void main()
{
   FragmentPosition = aPos;
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}