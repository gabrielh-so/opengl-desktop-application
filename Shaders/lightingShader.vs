#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat3 normalMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec3 Normal;
out vec3 FragmentPosition;

void main()
{
    // apply the various matricies for transforming the vetexes into viewspace
    gl_Position = perspective * view * model * vec4(position, 1.0f);

    // calculate the world space of the fragment for the fragment shader to compare

    FragmentPosition = vec3(view * model * vec4(position, 1.0f));

    // apply the normal matrix to the normal and pass to the fragment shader
    Normal = normalMatrix * normal;
}