#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "windowmanager.h"
#include "texturemanager.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "lightmanager.h"

class Renderer {
public:
    void init();
    void display(Camera* camPtr, WindowManager* wmPtr);
    void display(Camera* camPtr, WindowManager* wmPtr, const std::vector<Mesh*>& meshesToDraw);
    void setSunlight(const DirLight& light);
    void destroy();
private:
    std::vector<Shader> shaders;
    std::vector<Mesh> meshes;
    LightManager lightManager;
    
};
