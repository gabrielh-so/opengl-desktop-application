#version 330 core
#define POINT_LIGHT_COUNT 4

out vec4 FragColour;

// Material is a struct that contains all the information
// the lighting shader needs to process lighting properly
struct Material
{
    // remove the ambient colour because it's equal to the diffuse colour anyway
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;

};

struct Spotlight
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;

};

struct DirectionLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

uniform Material material;
uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform DirectionLight directionLight;
uniform Spotlight spotlight;

in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TextureCoordinates;

// uniform vec3 lightPosition;

// lighting in view space therefore view position is origin
uniform vec3 viewPosition;

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);
vec3 CalculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewDirection);
vec3 CalculateSpotlight(Spotlight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);

void main()
{

    vec3 viewDirection = normalize(viewPosition - FragmentPosition);

    vec3 normal = normalize(Normal);

    vec3 result = CalculateDirectionLight(directionLight, normal, viewDirection);

    for (int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        result += CalculatePointLight(pointLights[i], normal, FragmentPosition, viewDirection);
    }
    
    result += CalculateSpotlight(spotlight, normal, FragmentPosition, viewDirection);

    FragColour = vec4(result, 1.0f);
    
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection){
    ///////////////////////////
    //        ambient        //
    ///////////////////////////

    // vec3 ambient = material.ambient * lightColour;
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoordinates));

    ///////////////////////////
    //        diffuse        //
    ///////////////////////////

    // find the vector from the fragment position to the light source
    vec3 lightDirection = normalize(light.position - fragmentPosition);

    float diffuseFloat = max( dot(normal, lightDirection), 0.0f);

    vec3 diffuse = light.diffuse * diffuseFloat * vec3(texture(material.diffuse, TextureCoordinates));  

    
    ///////////////////////////
    //       specular        //
    ///////////////////////////

    // find the vector from the fragment position to the view position (assumed 0 if lighting in view space)
    // view position is origin therefore the direction is the negative fragment position vector
    // vec3 viewDirection = normalize(-FragmentPosition);

    // reflect the light direction against the normal to get the reflection direction
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    // get the angle between the reflection and the view direction, confirm it's not negative, and then bring it to the power
    float specularFloat = pow( max( dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    vec3 specular = light.specular * specularFloat * vec3(texture(material.specular, TextureCoordinates) + 0.1f);


    // calculate the attenuation of the light
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    //ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // summing up all the values

    return (diffuse + specular);
}

vec3 CalculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewDirection)
{
    ///////////////////////////
    //        ambient        //
    ///////////////////////////

    // vec3 ambient = material.ambient * lightColour;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoordinates));


    ///////////////////////////
    //        diffuse        //
    ///////////////////////////

    // find the vector from the fragment position to the light source
    vec3 lightDirection = normalize(-light.position);

    float diffuseFloat = max( dot(normal, lightDirection), 0.0f);

    vec3 diffuse = light.diffuse * diffuseFloat * vec3(texture(material.diffuse, TextureCoordinates));  

    
    ///////////////////////////
    //       specular        //
    ///////////////////////////

    // find the vector from the fragment position to the view position (assumed 0 if lighting in view space)
    // view position is origin therefore the direction is the negative fragment position vector
    // vec3 viewDirection = normalize(-FragmentPosition);

    // reflect the light direction against the normal to get the reflection direction
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    // get the angle between the reflection and the view direction, confirm it's not negative, and then bring it to the power
    float specularFloat = pow( max( dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    vec3 specular = light.specular * specularFloat * vec3(texture(material.specular, TextureCoordinates) + 0.1f);

    // summing up all the values

    return (ambient + diffuse + specular);
}

vec3 CalculateSpotlight(Spotlight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection)
{
    ///////////////////////////
    //        ambient        //
    ///////////////////////////

    // vec3 ambient = material.ambient * lightColour;
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoordinates));

    // find the vector from the fragment position to the light source
    vec3 lightDirection = normalize(light.position - fragmentPosition);

    // get the angle between the direction the spotlight is pointing and the relative direction of the light's position
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float spotlightIntensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);    

    ///////////////////////////
    //        diffuse        //
    ///////////////////////////


    float diffuseFloat = max( dot(normal, lightDirection), 0.0f);

    vec3 diffuse = light.diffuse * diffuseFloat * vec3(texture(material.diffuse, TextureCoordinates));  

    
    ///////////////////////////
    //       specular        //
    ///////////////////////////

    // find the vector from the fragment position to the view position (assumed 0 if lighting in view space)
    // view position is origin therefore the direction is the negative fragment position vector
    // vec3 viewDirection = normalize(-fragmentPosition);

    // reflect the light direction against the normal to get the reflection direction
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    // get the angle between the reflection and the view direction, confirm it's not negative, and then bring it to the power
    float specularFloat = pow( max( dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    vec3 specular = light.specular * specularFloat * vec3(texture(material.specular, TextureCoordinates) + 0.1f);
    
    vec3 emission = vec3(texture(material.emission, TextureCoordinates));


    // calculate the attenuation of the light
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    //ambient *= attenuation;
    diffuse *= attenuation * spotlightIntensity;
    specular *= attenuation * spotlightIntensity;

    // summing up all the values

    return (diffuse + specular);
}