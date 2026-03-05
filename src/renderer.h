#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "windowmanager.h"
#include "texturemanager.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"

class Renderer {
public:
    void init(int width, int height);
    void display(Camera* camPtr, WindowManager* wmPtr);
    void display(Camera* camPtr, WindowManager* wmPtr, const std::vector<Mesh*>& meshesToDraw);
    void destroy();
private:
    std::vector<Shader> shaders;
    std::vector<Mesh> meshes;
    
};
