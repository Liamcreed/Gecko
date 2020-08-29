#version 330 core
out vec4 FragColor;

struct Material
{
    sampler2D albedoMap;
    sampler2D roughnessMap;
    sampler2D AOMap;
    sampler2D normalMap;
    sampler2D metallicMap;

    vec3 albedo;
    float roughness;
    float metallic;
    float AO;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;

void main()
{   
    FragColor = texture(material.albedoMap, TexCoords);
}

