#include "Include/PointLight.h"

void PointLight::AssignShaderValues(std::string uniformLocation, Shader &shader)
{
    // uniform location should be defined something like "pointLights[0]." for attribute names to be appended on the end

    shader.UseShader();

    shader.SetFloatVec3Uniform(uniformLocation + "position", position);

    shader.SetFloatVec3Uniform(uniformLocation + "ambient", ambient);
    shader.SetFloatVec3Uniform(uniformLocation + "diffuse", diffuse);
    shader.SetFloatVec3Uniform(uniformLocation + "specular", specular);

    shader.SetFloatUniform(uniformLocation + "constant", constant);
    shader.SetFloatUniform(uniformLocation + "linear", linear);
    shader.SetFloatUniform(uniformLocation + "quadratic", quadratic);
}