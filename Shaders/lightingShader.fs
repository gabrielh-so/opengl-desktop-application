#version 330 core
out vec4 FragColour;

// Material is a struct that contains all the information
// the lighting shader needs to process lighting properly
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

// uniform vec3 lightPosition;

// lighting in view space therefore view position is origin
//uniform vec3 viewPosition;

uniform vec3 objectColour;
uniform vec3 lightColour;

void main()
{
    
    // get the normalized value of the normal
    vec3 normal = normalize(Normal);


    ///////////////////////////
    //        ambient        //
    ///////////////////////////

    // vec3 ambient = material.ambient * lightColour;
    vec3 ambient;


    ///////////////////////////
    //        diffuse        //
    ///////////////////////////

    // find the vector from the fragment position to the light source
    vec3 lightDirection = normalize(light.position - FragmentPosition);

    float diffuseFloat = max( dot(normal, lightDirection), 0.0f);

    vec3 diffuse = material.diffuse * diffuseFloat * lightColour;

    
    ///////////////////////////
    //       specular        //
    ///////////////////////////

    // find the vector from the fragment position to the view position (assumed 0 if lighting in view space)
    // view position is origin therefore the direction is the negative fragment position vector
    // vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 viewDirection = normalize(-FragmentPosition);

    // reflect the light direction against the normal to get the reflection direction
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    // get the angle between the reflection and the view direction, confirm it's not negative, and then bring it to the power
    float specularFloat = pow( max( dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    vec3 specular = material.specular * specularFloat * lightColour;

    // summing up all the values
    ambient = light.ambient * material.ambient * lightColour;
    diffuse *= light.diffuse * material.ambient;
    specular *= light.specular * material.ambient;

    vec3 result = (ambient + diffuse + specular) * objectColour;

    FragColour = vec4(result, 1.0f);
}