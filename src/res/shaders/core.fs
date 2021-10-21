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
    color = vec4(vertexColor, 1.0f);
    vec3 lightDirection = normalize(LightPos - FragPos);
    vec3 viewDirection = normalize(ViewPos - FragPos);
}