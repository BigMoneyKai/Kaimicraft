#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "debug.h"

void printShaderLog(unsigned int shader) {
    int len = 0;
    int chWritten = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

    if(len > 0) {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWritten, log);
        INFO("Shader info[%s]", log);
        free(log);
    }
}

void printProgramLog(int program) {
    int len = 0;
    int chWritten = 0;
    char* log;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = (char*)malloc(len);
        glGetProgramInfoLog(program, len, &chWritten, log);
        INFO("Program info[%s]", log);
        free(log);
    }
}

bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while(glErr != GL_NO_ERROR) {
        ERROR("OpenGL[%#x]", glErr);
        foundError =true;
        glErr = glGetError();
    }
    return foundError;
}
