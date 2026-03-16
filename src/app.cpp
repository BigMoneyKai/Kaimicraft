#include "app.h"

void App::init() {
    title = "Minecraft";
    isRunning  =false;

    drawLogo();

    INFO("-----Thread pool initailizating-----");
    threadPool.init();
    INFO("Thread pool ready");
    std::cout << '\n';

    INFO("-----Window manager initailizating-----");
    windowManager.init(title, FULLSCREEN);
    headless = windowManager.isHeadless();
    INFO("Window manager ready");
    std::cout << '\n';

    if (headless) {
        INFO("Headless mode active: skipping graphics initialization and game loop");
        return;
    }

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
    atlas.init("assets/texture/atlas.png");
    sun.init("assets/texture/sun.png");
    INFO("Texture manager ready");
    std::cout << '\n';

    INFO("-----Renderer initailizating-----");
    renderer.init();
    INFO("Renderer ready");
    std::cout << '\n';

    INFO("-----Terrain generator initailizating-----");
    tg.init();
    INFO("Terrain generator ready");
    std::cout << '\n';

    INFO("-----Chunk manager initailizating-----");
    chunkManager.init();
    INFO("Chunk manager ready");
    std::cout << '\n';

    INFO("-----Camera initailizating-----");
    camera.init();
    INFO("Camera ready");
    std::cout << '\n';

    float spawnX = Util::randomNum(-1000.0f, 1000.0f);
    float spawnZ = Util::randomNum(-1000.0f, 1000.0f);
    int height = tg.getHeight((int)spawnX, (int)spawnZ);
    float spawnY = (float)height + 1.0f + Util::playerEyeHeight() + 0.05f;
    camera.setPos(glm::vec3(spawnX, spawnY, spawnZ));

    glfwSetInputMode(windowManager.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    INFO("Set input mode: %s", "cursor hidden");
    std::cout << '\n';

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(windowManager.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        INFO("Raw mouse motion enabled");
    }

    glfwSwapInterval(1);
}

void App::run() {
    if (headless) {
        INFO("Headless mode active: run skipped");
        return;
    }
    glfwSwapInterval(1);

    INFO("Game loop running...");
    gameLoop();
    INFO("Game loop quit");
}

void App::shutdown() {
    windowManager.destroy();
    if (headless) {
        return;
    }
    inputManager.destroy();
    timeManager.destroy();
    atlas.destroy();
    renderer.destroy();
    camera.destroy();
    chunkManager.destroy();
    tg.destroy();
}

void App::gameLoop() {
    isRunning = true;
    while(!glfwWindowShouldClose(windowManager.window) && isRunning) {
        timeManager.update();
        renderer.setSunlight(timeManager.sunlight());

        glClear(GL_DEPTH_BUFFER_BIT);
        const glm::vec3& sky = timeManager.skyColor();
        glClearColor(sky.r, sky.g, sky.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        atlas.bind();
        chunkManager.updateDynamic(camera.pos(), &tg, threadPool);
        renderer.display(&camera, &windowManager, chunkManager.meshes());

        glfwPollEvents();
        inputManager.poll();
        Util::inputControl(this);
        inputManager.nextFrame();

        glfwSwapBuffers(windowManager.window);
    }
}
