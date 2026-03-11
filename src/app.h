#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "debug.h"
#include "logo.h"
#include "windowmanager.h"
#include "inputmanager.h"
#include "timemanager.h"
#include "texturemanager.h"
#include "renderer.h"
#include "camera.h"
#include "util.h"
#include "chunkmanager.h"
#include "terraingenerator.h"

class App {
public:
    void init(); 
    void run(); 
    void shutdown();

    WindowManager windowManager;
    InputManager inputManager;
    TimeManager timeManager;
    TextureManager atlas;
    TextureManager sun;
    Renderer renderer;
    Camera camera;
    ChunkManager chunkManager;
    TerrainGenerator tg;

    bool isRunning;
    bool headless = false;

private:
    void gameLoop();

private:
    std::string title; 
    
};
