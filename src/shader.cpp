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

int Shader::getLocation(const std::string& name) {
    if(uniformCache.count(name)) {
        return uniformCache[name];
    }

    int loc = glGetUniformLocation(shaderProgram, name.c_str());
    uniformCache[name] = loc;
    return loc;
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

// mat4
void Shader::setMat(const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(
        getLocation(name),
        1,
        GL_FALSE,
        glm::value_ptr(mat)
    );
}

// mat3
void Shader::setMat(const std::string& name, const glm::mat3& mat) {
    glUniformMatrix3fv(
        getLocation(name),
        1,
        GL_FALSE,
        glm::value_ptr(mat)
    );
   
}

void Shader::setVec(const std::string& name, const glm::vec4& vec) {
    glUniform4fv(
        getLocation(name),
        1,
        glm::value_ptr(vec)
    );

}

void Shader::setVec(const std::string& name, const glm::vec3& vec) {
    glUniform3fv(
        getLocation(name),
        1,
        glm::value_ptr(vec)
    );
}

void Shader::setVec(const std::string& name, const glm::vec2& vec) {
    glUniform2fv(
        getLocation(name),
        1,
        glm::value_ptr(vec)
    );
}

void Shader::setFloat(const std::string& name, const float& val) {
    glUniform1f(
        getLocation(name),
        val
    );
}

void Shader::setInt(const std::string& name, const int& val) {
    glUniform1i(
        getLocation(name),
        val
    );
}
