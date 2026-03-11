#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "debug.h"

class Shader {
public: 
    void init(const std::string& vertPath, const std::string& fragPath);
    void process();
    void destroy();
    
    void setMat(const std::string& name, const glm::mat4& mat);
    void setMat(const std::string& name, const glm::mat3& mat);
    void setVec(const std::string& name, const glm::vec4& vec);
    void setVec(const std::string& name, const glm::vec3& vec);
    void setVec(const std::string& name, const glm::vec2& vec);
    void setFloat(const std::string& name, const float& val);
    void setInt(const std::string& name, const int& val);

    unsigned int shaderProgram;
private:
    std::string parse(const std::string& shaderPath);
    unsigned int compile(const std::string& src, GLenum type);
    int getLocation(const std::string& name);
private:
    std::string vertPath;
    std::string fragPath;

    unsigned int vertShader;
    unsigned int fragShader;

    std::string vertShaderSrc;
    std::string fragShaderSrc;

    std::unordered_map<std::string, int> uniformCache;
};
