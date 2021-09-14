#include <iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

const GLchar *vertexShaderCode = "#version 330 core\n"
								 "layout(location=0) in vec3 position;\n"
								 "void main()\n"
								 "{\n"
								 "gl_Position=vec4(position,1.0f);\n"
								 "}"; //顶点着色器
const GLchar *fragmentShaderCode = "#version 330 core\n"
								   "out vec4 color;\n"
								   "void main()\n"
								   "{\n"
								   "color=vec4(1.0f,0.5f,0.2f,1.0f);\n"
								   "}"; //片段着色器

GLint success;		 //存储编译是否正确
GLchar infoLog[512]; //存储日志

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

	//开始导入着色器
	//首先是顶点着色器
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL); //传 1 个着色器，传入先前的代码，传入位置为 NULL（起始位置）
	glCompileShader(vertexShader);							  //编译 vertexShader

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //iv 代表可以返回多个参数。这里是查看 vertexShader 的编译状态
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	//然后是片段着色器
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	//链接
	GLuint shaderProgram = glCreateProgram(); //直接在 GPU 上运行的代码
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKONG_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); //释放空间
	glDeleteShader(fragmentShader);

	//顶点信息
	//OpenGL 的原点在屏幕正中心
	GLfloat vertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f};

	GLuint VAO, VBO; //顶点数组对象，顶点缓冲对象
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);															   //绑定 VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);												   //绑定 VBO，告诉 OpenGL 这是个顶点缓冲对象
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		   //指定 ID 的缓冲类型为 GL_ARRAY_BUFFER，大小刚好为顶点大小，实际发出数据为顶点数组，OpenGL 处理策略为数据几乎不改变
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0); //要修改索引值为0 的顶点属性，修改 3 个，浮点，连续顶点偏移量为 3 个浮点数，组件偏移量为0
	glEnableVertexAttribArray(0);													   //允许着色器读取 GPU（服务端）的数据

	//解除绑定，防止误操作
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//绘制循环
	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, screenWidth, screenHeight); //左下角开始位置，右上角结束位置，把这部分显存划分到自己这里
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); //渲染

		glUseProgram(shaderProgram);	  //使用先前定义的着色器
		glBindVertexArray(VAO);			  //绑定完，可以画图了
		glDrawArrays(GL_TRIANGLES, 0, 3); //画三个点，从第0，到第2。画个三角形
		glBindVertexArray(0);			  //解除绑定

		glfwSwapBuffers(window); //交换缓冲区，在 window 上更新内容
	}
	glfwTerminate();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
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