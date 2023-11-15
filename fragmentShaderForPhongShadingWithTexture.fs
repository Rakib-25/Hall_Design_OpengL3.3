#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct PointLight {
    vec3 position;
    
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionLight {
    vec3 position;
    
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


#define NR_POINT_LIGHTS 2

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform PointLight pointLights[10];
uniform SpotLight spotLights[10];
uniform DirectionLight directionLights[10];
uniform Material material;

// function prototypes
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V);
vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V);
vec3 CalcDirectLight(Material material, DirectionLight light, vec3 N, vec3 fragPos, vec3 V);

vec3 spotPosition = vec3(-5.0,-5.0,-5.0);


//uniform vec3 viewPos;
//uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform Material material;

// function prototypes
// Sir // vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V);

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    
    vec3 result;
    // point lights
    
    result += CalcSpotLight(material, spotLights[0], N, FragPos, V);

    result += CalcPointLight(material, pointLights[0], N, FragPos, V);  
    result += CalcPointLight(material, pointLights[1], N, FragPos, V);  
    result += CalcPointLight(material, pointLights[2], N, FragPos, V);  
    result += CalcPointLight(material, pointLights[3], N, FragPos, V);
    result += CalcPointLight(material, pointLights[4], N, FragPos, V);  
    result += CalcPointLight(material, pointLights[5], N, FragPos, V);  
    

    result += CalcDirectLight(material, directionLights[0], N, FragPos, V);
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a point light.

vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
    // attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = vec3(texture(material.specular, TexCoords)) * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
    vec3 K_A = vec3(texture(material.diffuse, TexCoords));
    vec3 K_D = vec3(texture(material.diffuse, TexCoords)) ;
    vec3 K_S = vec3(texture(material.specular, TexCoords));
    
    // attenuation
    float d = length(light.position - fragPos);
    //float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    //for spotlight

    float attenuation = 0;

    float theta = cos(25.0);
    vec3 V_l = -normalize(light.position-spotPosition);
    vec3 V_o = normalize(fragPos-light.position);

    if(dot(V_l,V_o) >= theta){
        attenuation = pow(dot (V_l, V_o),12);
    }
    else
    {
        attenuation  = 0;
    }


    float intensity = 1;


    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = K_S * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation ;
    diffuse *= attenuation ;
    specular *= attenuation ;
    
    return (ambient + diffuse + specular);
}




















// calculates the color when using a point light.
vec3 CalcDirectLight(Material material, DirectionLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
    vec3 K_A = vec3(texture(material.diffuse, TexCoords));
    vec3 K_D = vec3(texture(material.diffuse, TexCoords)) ;
    vec3 K_S = vec3(texture(material.specular, TexCoords));
    
    // attenuation
    float d = length(light.position - fragPos);
    //float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    float attenuation = 0;

    if(d>0.5){
        attenuation = 1.0;
    }
    else
    {
        attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    }
    
    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = K_S * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}