#include <bits/stdc++.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "./utils/util.hpp"
#include "./utils/Window.hpp"
#include "./classes/Transform.hpp"
#include "./classes/Object.hpp"
#include "./classes/Camera.hpp"

using namespace std;

void keyHandler(GLFWwindow* win);
void display(GLFWwindow* win, GLuint rProgram, GLuint vao, double currentTime);

unique_ptr<Object> Delaunay;
unique_ptr<Camera> camera;

void display(
    Window& win,
    const double& currentTime
) {
    win.clear();

    auto fTime{static_cast<float>(currentTime)};
    glm::mat4 model{1.f};
    model = glm::translate(model, { cos(currentTime)*.5f, 0, 0 });
    model = glm::rotate(model, fTime, { 0, 0, 1 });
    model = glm::rotate(model, fTime/2, { 0, 1, 0 });
    model = glm::rotate(model, fTime/3, { 1, 0, 0 });

    Utils::setProjection(Delaunay->getDefaultProgram(), win.getPerspective());
    camera->setView(Delaunay->getDefaultProgram());
    Delaunay->applyColorFilter();
    Delaunay->applyTransform(glm::transpose(model));
    Delaunay->draw();

    win.update();
}

void keyHandler(Window& win) {
    if (glfwGetKey(win.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win.get(), GL_TRUE);
        return;
    }

    if (glfwGetKey(win.get(), GLFW_KEY_0) == GLFW_PRESS)
        camera->resetTransform();
    else {
        auto normDir{win.getDeltaTime()};
        if (glfwGetKey(win.get(), GLFW_KEY_W) == GLFW_PRESS)
            camera->move({ normDir, 0, 0 });
        if (glfwGetKey(win.get(), GLFW_KEY_S) == GLFW_PRESS)
            camera->move({ -normDir, 0, 0 });
        if (glfwGetKey(win.get(), GLFW_KEY_A) == GLFW_PRESS)
            camera->move({ 0, -normDir, 0 });
        if (glfwGetKey(win.get(), GLFW_KEY_D) == GLFW_PRESS)
            camera->move({ 0, normDir, 0 });
        if (glfwGetKey(win.get(), GLFW_KEY_Q) == GLFW_PRESS)
            camera->move({ 0, 0, -normDir });
        if (glfwGetKey(win.get(), GLFW_KEY_E) == GLFW_PRESS)
            camera->move({ 0, 0, normDir });
    }
}

void mouseHandler(Window& win) {
    auto last{win.getLastCursor()},
        cur{win.updateCursor()};
    camera->rotate({ cur.first - last.first, cur.second - last.second });
}

int main() {
    Window::initContext(16);

    Window window{
            { 800, 500 },
            "Teste",
            nullptr, nullptr };
    auto renderProgram{Utils::createRenderProgram("vShader.glsl", "fShader.glsl")};

    camera = make_unique<Camera>(glm::vec3{ 0, 0, -1 }, 2.f);

    Delaunay = make_unique<Object>(renderProgram);
    Delaunay->pushElement(
        GL_TRIANGLE_STRIP,
        { { -0.1f, 0.f, 0.f }, { 0.f, 0.1f, 0.f }, { 0.1f, 0.f, 0.f }, { 0, 0, 0.1f } },
        { { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 1 }, { 1, 1, 1, 1 } },
        { 0, 1, 2, 3, 0, 1 }
    );
    Delaunay->pushElement(
        GL_LINE_STRIP,
        { { -0.1f, 0.f, 0.f }, { 0.f, 0.1f, 0.f }, { 0.1f, 0.f, 0.f }, { 0, 0, 0.1f } },
        { { 0, 0, 0, 1 } },
        { 0, 1, 2, 3, 0, 2, 1, 3 }
    );

    window.clear({ 1., 0., 1., 1. });
    while (!glfwWindowShouldClose(window.get())) {
        display(window, glfwGetTime());
        glfwPollEvents();
        keyHandler(window);
        mouseHandler(window);

        auto err{glGetError()};
        if (err != GL_NO_ERROR) cerr << "OpenGL error: " << err << endl;
    }

    Utils::deletePrograms(renderProgram);

    return 0;
}
