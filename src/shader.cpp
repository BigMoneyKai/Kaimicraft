#include "shader.h"

void Shader::init(const std::string& vert, const std::string& frag) {
    vertPath = vert;
    fragPath = frag;
}

std::string Shader::parse(const std::string& shaderPath) {
    std::ifstream file(shaderPath);

    if(!file.is_open()) {
        ERROR("Failed to open shader file %s", shaderPath.c_str());
    }
    std::stringstream content;
    content << file.rdbuf();
    file.close();
    return content.str();
}

unsigned int Shader::compile(const std::string& src, GLenum type) {
    unsigned int shader = glCreateShader(type);
    const char* srcStr = src.c_str();
    glShaderSource(shader, 1, &srcStr, NULL);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    printShaderLog(shader);
    if(!success) {
        ERROR("shader compilation failed");
    }

    return shader;
}

void Shader::process() {
    vertShaderSrc = parse(vertPath);
    fragShaderSrc = parse(fragPath);

    vertShader = compile(vertShaderSrc, GL_VERTEX_SHADER);
    fragShader = compile(fragShaderSrc, GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    int success = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    printProgramLog(shaderProgram);
    if(!success) {
        ERROR("shader program linking failed");
    }

    destroy();
}

void Shader::destroy() {
    if(vertShader != 0) {
        glDeleteShader(vertShader);
        vertShader = 0;
    }
    if(fragShader != 0) {
        glDeleteShader(fragShader);
        fragShader = 0;
    }
}
