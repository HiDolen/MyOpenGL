# MyOpenGL

我的第一个 OpenGL 工程，在 windows10 上的 VSCode 运行。课程学习用。

- 配置完成了 GL、GLFW 和 SOIL2，修改了 tasks.json 使得能够运行 OpenGL。需要提前配置 c++ 环境。
- output 文件夹是必须的，否则不能运行。
- 程序使用静态编译，所以根目录下的 glew32.dll 和 glfw3.dll 暂时不会用上。
- lib 文件夹下的 libSOIL2.a 是在 SOIL2 源码文件夹下通过以下指令得到的：
    - ``gcc -c *.c``
    - ``ar rcs libSOIL.a *.o``
    - 注意修改 tasks.json 中的参数。-lSOIL2 要放在靠前的位置形成依赖。

```shell
gcc -c *.c
ar rcs libSOIL.a *.o
```

- 123