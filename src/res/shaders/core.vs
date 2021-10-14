#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 myColor;
layout(location = 2) in vec2 textCoord;
out vec3 vertexColor;
out vec2 outTextCoord;

uniform float time = 0;
void main()
{
    gl_Position = vec4(position, 1.0f);
    vertexColor = myColor; //将值传出
    //outTextCoord = vec2(textCoord.x, 1 - textCoord.y);

    if(time != 0)
    {
        outTextCoord = vec2(textCoord.x, 1 - textCoord.y + time * 0.1f);
    }
    else
    {
        outTextCoord = vec2(textCoord.x, 1 - textCoord.y);
    }
};