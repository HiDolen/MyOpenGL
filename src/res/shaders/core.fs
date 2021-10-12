#version 330 core
out vec4 color;
in vec3 vertexColor;
// in vec2 outTextCoord;

uniform float time;//uniform，另一种获得输入的关键字
// uniform sampler2D texture0;

void main()
{
    // color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    color = vec4(vertexColor, 1.0f);
    // color = texture(texture0, outTextCoord);
}