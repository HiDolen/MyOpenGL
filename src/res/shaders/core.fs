#version 330 core
out vec4 color;
in vec3 vertexColor;
in vec2 outTextCoord;

uniform float time;//uniform，另一种获得输入的关键字
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    // color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    // color = vec4(vertexColor, 1.0f);

    // color = texture(texture1, outTextCoord);
    vec4 color1 = texture(texture0, outTextCoord); 	//从纹理中采样出颜色值1  
    vec4 color2 = texture(texture1, outTextCoord); 	//从纹理中采样出颜色值2
    // color = color1 * (cos(time * 3.0f) + 1) * 0.5 +  color2 * (1 - (cos(time * 3.0f) + 1) * 0.5);
    color=color1;
}