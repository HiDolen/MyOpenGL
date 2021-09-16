#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
    GLuint vertex, fragment;

public:
    GLuint Program;

    Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
    {
        //读取文件代码
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }

        const GLchar *vShaderCode = vertexCode.c_str();
        const GLchar *fShaderCode = fragmentCode.c_str();

        //编译着色器
        GLint success;       //存储编译是否正确
        GLchar infoLog[512]; //存储日志

        //开始导入着色器
        //首先是顶点着色器

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);      //传 1 个着色器，传入先前的代码，传入位置为 NULL（起始位置）
        glCompileShader(vertex);                            //编译 vertexShader
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); //iv 代表可以返回多个参数。这里是查看 vertexShader 的编译状态
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        //然后是片段着色器
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        //链接
        this->Program = glCreateProgram(); //直接在 GPU 上运行的代码
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);

        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKONG_FAILED\n"
                      << infoLog << std::endl;
        }

        /* glDeleteShader(vertex); //释放空间
        glDeleteShader(fragment); */
    }
    ~Shader()
    {
        //析构
        glDetachShader(this->Program, vertex);
        glDetachShader(this->Program, fragment);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(this->Program);
    }

    void Use()
    {
        glUseProgram(this->Program);
    }
};