#version 330 core
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

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TextureCoordinates;

// uniform vec3 lightPosition;

// lighting in view space therefore view position is origin
uniform vec3 viewPosition;

void main()
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

    float diffuseFloat = max( dot(Normal, lightDirection), 0.0f);

    vec3 diffuse = light.diffuse * diffuseFloat * vec3(texture(material.diffuse, TextureCoordinates));  

    
    ///////////////////////////
    //       specular        //
    ///////////////////////////

    // find the vector from the fragment position to the view position (assumed 0 if lighting in view space)
    // view position is origin therefore the direction is the negative fragment position vector
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    // vec3 viewDirection = normalize(-FragmentPosition);

    // reflect the light direction against the normal to get the reflection direction
    vec3 reflectionDirection = reflect(-lightDirection, Normal);

    // get the angle between the reflection and the view direction, confirm it's not negative, and then bring it to the power
    float specularFloat = pow( max( dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    vec3 specular = light.specular * specularFloat * vec3(texture(material.specular, TextureCoordinates) + 0.1f);
    
    vec3 emission = vec3(texture(material.emission, TextureCoordinates));

    // summing up all the values

    FragColour = vec4(ambient + diffuse + specular + emission, 1.0f);
}