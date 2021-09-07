#include <iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

const GLchar *vertexShaderCode = "#version 330 core\n"
								 "layout(localtion=0) in vec3 position;\n"
								 "void main()\n"
								 "{\n"
								 "gl_Position=vec4(position,1.0f);\n"
								 "}"; //顶点着色器
const GLchar *fragmentShaderCode = "";

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
	glewExperimental = GL_TRUE;		//历史遗留问题，必须把这个设为 true
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, screenWidth, screenWidth); //左下角开始位置，右上角结束位置，把这部分显存划分到自己这里
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); //渲染
		glfwSwapBuffers(window);	  //交换缓冲区，在 window 上更新内容
	}
	glfwTerminate();
	return 0;

	/* glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // must for Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", nullptr,
										  nullptr);
	// next two lines are for mac retina display
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0; */
}