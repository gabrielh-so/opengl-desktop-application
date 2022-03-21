#include "Include/DirectionLight.h"

void DirectionLight::AssignShaderValues(std::string uniformLocation, Shader &shader)
{
    // uniform location should be defined something like "pointLights[0]." for attribute names to be appended on the end and values assigned

    shader.UseShader();

    shader.SetFloatVec3Uniform(uniformLocation + "position", position);

    shader.SetFloatVec3Uniform(uniformLocation + "ambient", ambient);
    shader.SetFloatVec3Uniform(uniformLocation + "diffuse", diffuse);
    shader.SetFloatVec3Uniform(uniformLocation + "specular", specular);
}