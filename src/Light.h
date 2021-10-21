#ifndef Light_h
#define Light_h

#include <GL/glew.h>

#include "Shader.h"

GLfloat vertices_light[] =
    {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f};

class Light
{
public:
    Light()
    {
        this->update();
    }
    void Draw(Shader &shader)
    {
        glBindVertexArray(this->VAO); //绑定完，可以画图了
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0); //解除绑定 VAO
    }

private:
    GLuint VAO, VBO;
    void update()
    {
        glGenVertexArrays(1, &this->VAO);                                                      //创建空间，创建 1 个空间
        glGenBuffers(1, &this->VBO);                                                           //创建空间，创建 1 个空间
        glBindVertexArray(this->VAO);                                                          //绑定 VAO
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);                                              //绑定 VBO，告诉 OpenGL 这是个顶点缓冲对象（GL_ARRAY_BUFFER）
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_light), vertices_light, GL_STATIC_DRAW); //将顶点数据复制到缓冲区，大小刚好为顶点大小，实际发出数据为顶点数组，OpenGL 处理策略为数据几乎不改变
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);     //告诉 OpenGL 怎么处理顶点数据。要修改索引值为0 的顶点属性，修改 3 个，浮点，连续顶点偏移量为 3 个浮点数，组件偏移量为0
        glEnableVertexAttribArray(0);                                                          //让 shader 能够读取数据

        glBindBuffer(GL_ARRAY_BUFFER, 0); //解除顶点缓冲对象 VBO 的绑定，防止误操作
        glBindVertexArray(0);             //解除绑定 VAO
    }
};

#endif