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
    amongus,
    cadeira,
    casa,
    chair
    // pato
    ;

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

    glm::mat4 cdModel{1.f};
    cdModel = glm::translate(cdModel, {0, 0, 5});
    cadeira->setupDefaultRenderer(camera, win);
    cadeira->applyColorFilter();
    cadeira->applyTransform(glm::transpose(cdModel));
    cadeira->draw();

    glm::mat4 amModel{1.f};
    amModel = glm::scale(amModel, {0.02, 0.02, 0.02});
    amModel = glm::translate(amModel, {100, 0, 100});
    amongus->setupDefaultRenderer(camera, win);
    amongus->applyColorFilter();
    amongus->applyTransform(glm::transpose(amModel));
    amongus->draw();

    glm::mat4 casaModel{1.f};
    casaModel = glm::scale(casaModel, {0.5, 0.5, 0.5});
    casa->setupDefaultRenderer(camera, win);
    casa->applyColorFilter();
    casa->applyTransform(glm::transpose(casaModel));
    casa->draw();

    // glm::mat4 patoModel{1.f};
    // patoModel = glm::scale(patoModel, {0.02, 0.02, 0.02});
    // patoModel = glm::translate(patoModel, {200, 200, 200});
    // pato->setupDefaultRenderer(camera, win);
    // pato->applyColorFilter();
    // pato->applyTransform(glm::transpose(patoModel));
    // pato->draw();

    glm::mat4 chair_model{1.f};
    chair_model = glm::translate(chair_model, {0, 0, 2});
    chair_model = glm::scale(chair_model, {0.2, 0.2, 0.2});
    chair->setupDefaultRenderer(camera, win);
    chair->applyColorFilter();
    chair->applyTransform(glm::transpose(chair_model));
    chair->draw();

    sun->emitLight({ 1.0, 0.8392, 0.2509 });
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

    amongus = make_unique<Object>(programs.at("defaultProgram"));
    if (!amongus->loadFromFile("../assets/amongus/amongus.obj", "../assets/amongus/amongus.png")) {
        cerr << "Error loading model!" << endl;
        Utils::deleteProgramDict(programs);
        return 1;
    }

    casa = make_unique<Object>(programs.at("defaultProgram"));
    if (!casa->loadFromFile("../assets/casa/house.obj", "../assets/casa/textures/DoorsWoodSingleOld0107_1_M.jpg")) {
        cerr << "Error loading model!" << endl;
        Utils::deleteProgramDict(programs);
        return 1;
    }

    // pato = make_unique<Object>(programs.at("defaultProgram"));
    // if (!pato->loadFromFile("../assets/pato/pato.obj", "../assets/pato/pato.png")) {
    //     cerr << "Error loading model!" << endl;
    //     Utils::deleteProgramDict(programs);
    //     return 1;
    // }

    chair = make_unique<Object>(programs.at("defaultProgram"));
    if (!chair->loadFromFile("../assets/cadeiraDentro/cadeiraDentro.obj", "../assets/cadeiraDentro/cadeiraDentro.png")) {
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
