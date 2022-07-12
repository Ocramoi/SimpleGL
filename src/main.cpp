#include <bits/stdc++.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./utils/util.hpp"
#include "./utils/Window.hpp"
#include "./utils/Settings.hpp"
#include "./classes/Transform.hpp"
#include "./classes/Object.hpp"
#include "./classes/Camera.hpp"

using namespace std;

void keyHandler(GLFWwindow* win);
void display(GLFWwindow* win, GLuint rProgram, GLuint vao, double currentTime);

unique_ptr<Camera> camera;
unique_ptr<Object> sun,
    cadeira;

void display(
    Window& win,
    const double& currentTime
) {
    win.clear();

    auto fTime{static_cast<float>(currentTime)};
    glm::mat4 model{1.f};
    model = glm::translate(model, { 10, 10, 10 });
    model = glm::scale(model, { 0.07, 0.07, 0.07 });
    model = glm::rotate(model, fTime, { 0, 0, 1 });
    model = glm::rotate(model, fTime/2, { 0, 1, 0 });
    model = glm::rotate(model, fTime/3, { 1, 0, 0 });

    sun->setupDefaultRenderer(camera, win);
    sun->applyColorFilter();
    sun->applyTransform(glm::transpose(model));
    sun->draw();

    cadeira->setupDefaultRenderer(camera, win);
    sun->emitLight({ 1.0, 0.8392, 0.2509 });
    cadeira->applyColorFilter();
    cadeira->applyTransform(glm::Identity4);
    cadeira->draw();

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
    Settings settings{"settings.conf"};

    if (settings.getValue<int>("SAMPLES").has_value())
        Window::initContext(settings.getValue<int>("SAMPLES").value());
    else
        Window::initContext(16);

    pair<float, float> size{
        settings.getValue<float>("WIDTH").has_value() ? settings.getValue<float>("WIDTH").value() : 800,
        settings.getValue<float>("HEIGHT").has_value() ? settings.getValue<float>("HEIGHT").value() : 500};
    Window window{
            size,
            "Teste",
            nullptr, nullptr };

    unordered_map<string, decltype(Utils::createRenderProgram("", ""))> programs;
    programs.emplace("defaultProgram", Utils::createRenderProgram("vShader.glsl", "fShader.glsl"));
    programs.emplace("lightSourceProgram", Utils::createRenderProgram("vShader.glsl", "fLightSourceShader.glsl"));

    camera = make_unique<Camera>(glm::vec3{ 0, 0, -1 }, 2.f);
    camera->setSensitivity(0.2f);

    cadeira = make_unique<Object>(programs.at("defaultProgram"));
    if (!cadeira->loadFromFile("../assets/bancoFora/banqueta.obj", "../assets/bancoFora/plastic-chair.jpg")) {
        cerr << "Error loading model!" << endl;
        Utils::deleteProgramDict(programs);
        return 1;
    }

    sun = make_unique<Object>(programs.at("lightSourceProgram"));
    if (!sun->loadFromFile("../assets/sun/sun.obj", "../assets/sun/sun.jpg")) {
        cerr << "Error loading model!" << endl;
        Utils::deleteProgramDict(programs);
        return 1;
    }

    window.clear({ .4588, .6666, 1., 1. });
    while (!glfwWindowShouldClose(window.get())) {
        display(window, glfwGetTime());
        glfwPollEvents();
        keyHandler(window);
        mouseHandler(window);

        auto err{glGetError()};
        if (err != GL_NO_ERROR) {
            cerr << "OpenGL error: " << err << endl;
            cerr << "\t" << gluErrorString(err) << endl;
        }
    }

    Utils::deleteProgramDict(programs);

    return 0;
}
