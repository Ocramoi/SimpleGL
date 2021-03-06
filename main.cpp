#include <bits/stdc++.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/util.hpp"
#include "utils/Window.hpp"
#include "classes/Transform.hpp"
#include "classes/Object.hpp"

using namespace std;

void keyHandler(GLFWwindow* win);
void display(GLFWwindow* win, GLuint rProgram, GLuint vao, double currentTime);

unique_ptr<Object> Delaunay;
glm::vec3 cameraInit{0, 0, -1},
    cameraPos{cameraInit};
float cameraStep{0.05f};

void display(
    Window& win,
    const double& currentTime
) {
    Window::clear();

    float fTime{static_cast<float>(currentTime)};
    glm::mat4 model{1.f};
    model *= glm::translate(model, { 0.1666667, 0, 0 });
    model *= glm::rotate(model, fTime, { 0, 0, 1 });
    model *= glm::rotate(model, fTime/2, { 0, 1, 0 });
    model *= glm::translate(model, { -0.1666667, 0, 0 });

    Utils::setProjection(Delaunay->getDefaultProgram(), win.getPerspective());
    Utils::setView(Delaunay->getDefaultProgram(), glm::translate(glm::Identity4, cameraPos));
    Delaunay->setColorFilter(glm::Identity4);
    Delaunay->applyColorFilter();
    Delaunay->setTransform(model);
    Delaunay->applyTransform();
    Delaunay->draw();

    glfwSwapBuffers(win.get());
}

void keyHandler(GLFWwindow* win) {
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, GL_TRUE);

    if (glfwGetKey(win, GLFW_KEY_0) == GLFW_PRESS)
        cameraPos = cameraInit;
    else {
        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos.x += cameraStep;
        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos.x -= cameraStep;
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos.y -= cameraStep;
        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos.y += cameraStep;
        if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
            cameraPos.z -= cameraStep;
        if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS)
            cameraPos.z += cameraStep;
    }
}

int main() {
    Window::initContext(16);

    auto window{Window(
            { 500, 500 },
            "Teste",
            nullptr, nullptr
        )};
    auto renderProgram{Utils::createRenderProgram("vShader.glsl", "fShader.glsl")};

    Delaunay = make_unique<Object>(renderProgram);
    vector<GLuint> els{ 0, 1, 2, 3, 0, 1 };
    Delaunay->pushElement(
        GL_TRIANGLE_STRIP,
        { { -0.1f, 0.f, 0.f }, { 0.f, 0.1f, 0.f }, { 0.1f, 0.f, 0.f }, { 0, 0, 0.1f } },
        { { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 1 }, { 1, 1, 1, 1 } },
        els
    );
    Delaunay->pushElement(
        GL_LINE_STRIP,
        { { -0.1f, 0.f, 0.f }, { 0.f, 0.1f, 0.f }, { 0.1f, 0.f, 0.f }, { 0, 0, 0.1f } },
        { { 0, 0, 0, 1 } },
        { 0, 1, 2, 3, 0, 2, 1, 3 }
    );

    Window::clear({ 1., 0., 1., 1. });
    while (!glfwWindowShouldClose(window.get())) {
        display(window, glfwGetTime());
        glfwPollEvents();
        keyHandler(window.get());

        auto err{glGetError()};
        if (err != GL_NO_ERROR) cerr << "OpenGL error: " << err << endl;
    }

    Utils::deletePrograms(renderProgram);

    return 0;
}
