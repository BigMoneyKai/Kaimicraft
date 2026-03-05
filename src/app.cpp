#include "app.h"

void App::init() {
    title = "Minecraft";
    width = 0;
    height = 0;
    isRunning  =false;
    
    drawLogo();

    INFO("-----Window manager initailizating-----");
    windowManager.init(title, FULLSCREEN);
    INFO("Window manager ready");
    std::cout << '\n';
    
    INFO("-----GLEW initailizating-----");
    if(glewInit() != GLEW_OK) {
        FATAL("GLEW initialization failed");
    }
    INFO("GLEW ready");
    std::cout << '\n';

    INFO("-----Input manager initailizating-----");
    inputManager.init(windowManager.window);
    INFO("Input manager ready");
    std::cout << '\n';
    
    INFO("-----Time manager initailizating-----");
    timeManager.init();
    INFO("Time manager ready");
    std::cout << '\n';
     
    INFO("-----Texture manager initailizating-----");
    textureManager.init("assets/texture/atlas.png");
    INFO("Texture manager ready");
    std::cout << '\n';
    
    INFO("-----Renderer initailizating-----");
    renderer.init(width, height);
    INFO("Renderer ready");
    std::cout << '\n';

    INFO("-----Terrain generator initailizating-----");
    tg.init();
    INFO("Terrain generator ready");
    std::cout << '\n';

    INFO("-----Chunk manager initailizating-----");
    chunkManager.init(0, 0);
    INFO("Chunk manager ready");
    std::cout << '\n';

    INFO("-----Camera initailizating-----");
    camera.init();
    INFO("Camera ready");
    std::cout << '\n';

    float spawnX = Util::randomNum(-1000.0f, 1000.0f);
    float spawnZ = Util::randomNum(-1000.0f, 1000.0f);
    int height = tg.getHeight((int)spawnX, (int)spawnZ);
    camera.setPos(glm::vec3(spawnX, (float)(height + 2), spawnZ));

    glfwSetInputMode(windowManager.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    INFO("Set input mode: %s", "cursor hidden");    
    std::cout << '\n';
    
    glfwSwapInterval(1);
}

void App::run() {
    glfwSwapInterval(1);

    INFO("Game loop running...");
    gameLoop();
    INFO("Game loop quit");
}

void App::shutdown() {
    windowManager.destroy();
    inputManager.destroy();
    timeManager.destroy();
    textureManager.destroy();
    renderer.destroy();
    camera.destroy();
    chunkManager.destroy();
    tg.destroy();
    glfwTerminate();
}

void App::gameLoop() {
    isRunning = true;
    while(!glfwWindowShouldClose(windowManager.window) && isRunning) {
        
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        
        textureManager.bind();
        chunkManager.updateDynamic(camera.pos(), &tg);
        renderer.display(&camera, &windowManager, chunkManager.meshes());
        
        glfwPollEvents();
        Util::inputControl(this);
        inputManager.nextFrame();

        glfwSwapBuffers(windowManager.window);
    }
}
 
