#include <iostream>
#include <math.h>
#include <algorithm>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Light.h"

#include <SOIL2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode); //准备对键盘输入作出响应，函数声明
void MouseCallback(GLFWwindow *window, double xOffset, double yOffset);            //鼠标位置输入
void ScrollCallback(GLFWwindow *window, double xPos, double yPos);                 //鼠标滚轮输入

void DoMovement();

bool keys[1024];
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;

bool firstMouse = true; //避免一开始的鼠标跳变

using namespace std;
//////////////////////////
glm::mat4 myTransform = glm::mat4(1.0f); //用于设定方块位置
//////////////////////////
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

    glfwSetKeyCallback(window, KeyCallback);         //键盘输入相关
    glfwSetCursorPosCallback(window, MouseCallback); //鼠标位置输入
    glfwSetScrollCallback(window, ScrollCallback);   //鼠标滚轮输入

    glewExperimental = GL_TRUE; //历史遗留问题，必须把这个设为 true
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialise GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST); //开启深度测试
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND); //开启混合测试
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader = Shader("src/res/shaders/core.vs", "src/res/shaders/core.fs");     //导入并实例化 shader
    Shader lightShader = Shader("src/res/shaders/light.vs", "src/res/shaders/light.fs"); //导入并实例化 shader

    Light lightModel = Light();

    //为三维给出数组
    GLfloat vertices[] = {
        //位置3个，颜色3个，法向量
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        ////
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        ////
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        ////
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        ////
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        ////
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    GLuint VAO, VBO;                                                                                       //顶点数组对象，顶点缓冲对象。两个 ID
    glGenVertexArrays(1, &VAO);                                                                            //创建空间，创建 1 个空间
    glGenBuffers(1, &VBO);                                                                                 //创建空间，创建 1 个空间
    glBindVertexArray(VAO);                                                                                //绑定 VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                                    //绑定 VBO，告诉 OpenGL 这是个顶点缓冲对象（GL_ARRAY_BUFFER）
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);                             //将顶点数据复制到缓冲区，大小刚好为顶点大小，实际发出数据为顶点数组，OpenGL 处理策略为数据几乎不改变
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *)0);                     //告诉 OpenGL 怎么处理顶点数据。要修改索引值为0 的顶点属性，修改 3 个，浮点，连续顶点偏移量为 3 个浮点数，组件偏移量为0
    glEnableVertexAttribArray(0);                                                                          //让 shader 能够读取数据
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat))); //读颜色
    glEnableVertexAttribArray(1);                                                                          //让 shader 能够读取数据
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat))); //读法向量
    glEnableVertexAttribArray(2);                                                                          //让 shader 能够读取数据

    glBindBuffer(GL_ARRAY_BUFFER, 0); //解除顶点缓冲对象 VBO 的绑定，防止误操作
    glBindVertexArray(0);             //解除绑定 VAO

    glm::vec3 lightPos = glm::vec3(0.0f, 1.5f, 0.0f);

    //绘制循环
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glViewport(0, 0, screenWidth, screenHeight);        //左下角开始位置，右上角结束位置，把这部分显存划分到自己这里
        glfwPollEvents();                                   //监听键盘事件
        DoMovement();                                       //摄像机视角变换
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);               //指定背景颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //设置背景颜色
        lightPos = glm::rotate(lightPos, glm::radians(0.05f), glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 view = camera.GetViewMatrix();                                                                                         //从相机获取视角
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f); //投影
        glm::mat4 transform = glm::mat4(1.0f);                                                                                           //初始化位置，使得可以旋转
        transform = glm::translate(transform, lightPos);
        transform = glm::scale(transform, glm::vec3(0.1f, 0.1f, 0.1f));

        lightShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        lightModel.Draw(lightShader);

        ourShader.Use(); //之后的着色渲染都会使用这个 shader 程序
        transform = myTransform;
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -2.0f));
        transform = glm::rotate(transform, glm::radians(20.0f) * static_cast<GLfloat>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f)); //彩色方块旋转
        // transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

        GLuint transLoc = glGetUniformLocation(ourShader.Program, "transform");
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glUniform3f(glGetUniformLocation(ourShader.Program, "ViewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(ourShader.Program, "LightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform1f(glGetUniformLocation(ourShader.Program, "material.diffuse"), 0.4f);
        glUniform1f(glGetUniformLocation(ourShader.Program, "material.specular"), 0.6f);

        glBindVertexArray(VAO); //绑定完，可以画图了
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //解除绑定 EBO
        glBindVertexArray(0);                     //解除绑定 VAO

        glfwSwapBuffers(window); //交换缓冲区，在 window 上更新内容
    }
    glfwTerminate();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        // camera.ProcessKeyboard(FORWARD, deltaTime);
        myTransform = glm::translate(myTransform, glm::vec3(0.0f, 0.05f, 0.0f));
    }
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        // camera.ProcessKeyboard(BACKWARD, deltaTime);
        myTransform = glm::translate(myTransform, glm::vec3(0.0f, -0.05f, 0.0f));
    }
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        // camera.ProcessKeyboard(LEFT, deltaTime);
        myTransform = glm::translate(myTransform, glm::vec3(-0.05f, 0.0f, 0.0f));
    }
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        // camera.ProcessKeyboard(RIGHT, deltaTime);
        myTransform = glm::translate(myTransform, glm::vec3(0.05f, 0.0f, 0.0f));
    }
}

void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    camera.ProcessMouseScroll(yOffset);
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    if (firstMouse) //第一次变换被忽略，阻止跳变
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}