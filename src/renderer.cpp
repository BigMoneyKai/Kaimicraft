#include "renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void Renderer::init() {
    meshes.emplace_back();
    meshes.back().init();

    shaders.emplace_back();
    shaders.back().init(
            "assets/shader/vertshader.glsl",
            "assets/shader/fragshader.glsl");
    shaders.back().process();
}

void Renderer::display(Camera* camPtr, WindowManager* wmPtr) {
    std::vector<Mesh*> meshPtrs;
    meshPtrs.reserve(meshes.size());
    for(auto& mesh : meshes) {
        meshPtrs.push_back(&mesh);
    }
    display(camPtr, wmPtr, meshPtrs);
}

void Renderer::display(Camera* camPtr, WindowManager* wmPtr, const std::vector<Mesh*>& meshesToDraw) {
    if(shaders.empty() || meshesToDraw.empty()) {
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // 默认但建议写
    const auto& shader = shaders[0];
    if(shader.shaderProgram == 0) {
        return;
    }

    glUseProgram(shader.shaderProgram);

    // Build projection matrix
    float aspect = (float)wmPtr->getWidth()/(float)wmPtr->getHeight();
    glm::mat4 pMat = camPtr->getProjMatrix(aspect);
    glm::mat4 vMat = camPtr->getViewMatrix();
    
    glm::mat4 mMat = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -10.0f, -5.0f));
    
    glm::mat4 mvMat = vMat * mMat;

    int mvLoc = glGetUniformLocation(shader.shaderProgram, "mv");
    if(mvLoc != -1) {
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    }
    int projLoc = glGetUniformLocation(shader.shaderProgram, "p");
    if(projLoc != -1) {
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    }

    int texLoc = glGetUniformLocation(shaders.back().shaderProgram, "uTexture1");
    glUniform1i(texLoc, 0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDepthFunc(GL_LEQUAL);

    for(auto* mesh: meshesToDraw) {
        if(mesh) {
            mesh->draw();
        }
    }
}

void Renderer::destroy() {
    for(auto mesh: meshes) {
        mesh.destroy();
    }
    for(auto shader: shaders) {
        shader.destroy();
    }
}
