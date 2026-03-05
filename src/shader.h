#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "debug.h"

class Shader {
public: 
    void init(const std::string& vertPath, const std::string& fragPath);
    void process();
    void destroy();
    
    unsigned int shaderProgram;
private:
    std::string parse(const std::string& shaderPath);
    unsigned int compile(const std::string& src, GLenum type);
private:
    std::string vertPath;
    std::string fragPath;

    unsigned int vertShader;
    unsigned int fragShader;

    std::string vertShaderSrc;
    std::string fragShaderSrc;

};
