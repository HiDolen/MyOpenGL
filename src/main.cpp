#include <iostream>
#include <math.h>
#include <algorithm>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <SOIL2/SOIL2.h>

const GLint WIDTH = 800, HEIGHT = 600;

using namespace std;

int main()
{
    glfwInit();

    //设置最低版本为 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //向前兼容。macOS 必须写上

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //不可变窗口大小

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "My First OpenGL", nullptr, nullptr); //创建窗口和它的 OpenGL 上下文
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //为高分辨率屏幕准备
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight); //获取窗口的大小

    glfwMakeContextCurrent(window); //设置当前窗口上下文
    glewExperimental = GL_TRUE;     //历史遗留问题，必须把这个设为 true
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialise GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    Shader myShader = Shader("src/res/shaders/core.vs", "src/res/shaders/core.fs"); //导入并实例化 shader

    //另一种方案画正方形
    GLfloat vertices[] =
        {
            //位置3个，颜色3个，纹理坐标2个
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   //右上角
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  //右下角
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //左下角
            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f   //左上角
        };
    unsigned int indices[] = //连接指数
        {
            0,
            1,
            3,
            1,
            2,
            3,
        };

    GLuint VAO, VBO;                                                                                       //顶点数组对象，顶点缓冲对象。两个 ID
    glGenVertexArrays(1, &VAO);                                                                            //创建空间，创建 1 个空间
    glGenBuffers(1, &VBO);                                                                                 //创建空间，创建 1 个空间
    glBindVertexArray(VAO);                                                                                //绑定 VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                                    //绑定 VBO，告诉 OpenGL 这是个顶点缓冲对象（GL_ARRAY_BUFFER）
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);                             //将顶点数据复制到缓冲区，大小刚好为顶点大小，实际发出数据为顶点数组，OpenGL 处理策略为数据几乎不改变
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);                     //告诉 OpenGL 怎么处理顶点数据。要修改索引值为0 的顶点属性，修改 3 个，浮点，连续顶点偏移量为 3 个浮点数，组件偏移量为0
    glEnableVertexAttribArray(0);                                                                          //让 shader 能够读取数据
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat))); //读颜色
    glEnableVertexAttribArray(1);                                                                          //让 shader 能够读取数据
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat))); //读纹理坐标
    glEnableVertexAttribArray(2);                                                                          //让 shader 能够读取数据

    GLuint EBO;            //顶点信息
    glGenBuffers(1, &EBO); //创建空间
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //解绑

    glBindBuffer(GL_ARRAY_BUFFER, 0); //解除顶点缓冲对象 VBO 的绑定，防止误操作
    glBindVertexArray(0);             //解除绑定 VAO

    GLuint texture; //纹理
    int width, height;
    glGenTextures(1, &texture);            //创建纹理
    glBindTexture(GL_TEXTURE_2D, texture); //绑定纹理，类型为二维纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    unsigned char *image = SOIL_load_image("src/res/images/T_image1.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0); //解除纹理绑定

    //绘制循环
    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();
        glViewport(0, 0, screenWidth, screenHeight); //左下角开始位置，右上角结束位置，把这部分显存划分到自己这里
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //指定背景颜色
        glClear(GL_COLOR_BUFFER_BIT);         //设置背景颜色

        myShader.Use();               //之后的着色渲染都会使用这个 shader 程序
        glActiveTexture(GL_TEXTURE0); //其实第 0 个纹理会被自动激活，不需要这行代码
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(myShader.Program, "texture0"), 0);

        glUniform1f(glGetUniformLocation(myShader.Program, "time"), time);

        glBindVertexArray(VAO); //绑定完，可以画图了
        // glDrawArrays(GL_TRIANGLES, 0, 6); //画六个点，从第0，到第2。画两个三角形
        //上面这个绘图方式不适于带有连接信息的绘图
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);          //绑定 EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //绘图
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);            //解除绑定 EBO
        glBindVertexArray(0);                                //解除绑定 VAO

        glfwSwapBuffers(window); //交换缓冲区，在 window 上更新内容
    }
    glfwTerminate();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
    return 0;
}