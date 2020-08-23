#version 330 core
out vec4 FragColor;

struct Material
{
    sampler2D albedoTexture;
    sampler2D roughnessTexture;
    sampler2D AOTexture;
    sampler2D normalTexture;
    sampler2D metallicTexture;

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
    FragColor = texture(material.albedoTexture, TexCoords);
}

