#version 330 core
out vec4 color;
in vec3 vertexColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 LightPos;
uniform vec3 ViewPos;

uniform float time;//uniform，另一种获得输入的关键字

struct Material
{
    float diffuse;
    float specular;
};

uniform Material material;

void main()
{
    // color = vec4(vertexColor, 1.0f);
    vec3 lightDirection = normalize(LightPos - FragPos);
    vec3 viewDirection = normalize(ViewPos - FragPos);
    vec3 norm = normalize(Normal);

    vec3 ambient = 0.2f * vertexColor;

    float diff = material.diffuse * max(dot(norm, lightDirection), 0.0f);//漫反射
    vec3 diffuse = diff * vertexColor;

    vec3 halfAngle = normalize(viewDirection + lightDirection);//反射
    float spec = material.specular * pow(max(dot(norm, halfAngle), 0.0f), 64.0f);
    vec3 specular = spec * vertexColor;

    color = vec4(ambient + diffuse +specular, 1.0f);
}