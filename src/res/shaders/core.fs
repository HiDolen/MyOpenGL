#version 330 core
out vec4 color;
in vec3 vertexColor;
uniform float time;//uniform，另一种获得输入的关键字

void main()
{
    color = vec4(cos(time * 5) * 0.5f + 0.5f, (1 - cos(time * 5) * 0.5f - 0.5f) * 0.6f, 0.0, 1.0);
}