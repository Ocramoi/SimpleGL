#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <algorithm>
#include <iostream>
#include <array>

#include "../utils/util.hpp"

using std::pair;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::array;

class Window {
    private:
        GLFWwindow* _win;
        float FOV{60.f}, zMin{0.01f}, zMax{1000.f},
                deltaTime{1.f};
        decltype(glfwGetTime()) lastFrame{0};
        glm::mat4 perspective{1.f};
        pair<float, float> wSize;
        bool enable3d;
        pair<double, double> lastCursor{0, 0};
    public:
        static void initContext(size_t samples);
        void setupInput();
        void clear();
        void clear(const array<float, 4>& Color);
        GLFWwindow* get();
        Window(
            const pair<float, float>& size,
            const string& title,
            GLFWmonitor *monitor,
            GLFWwindow *share,
            bool context3d = true
        );
        ~Window();
        void setPerspective(const glm::mat4& Perspective = glm::Identity4);
        void setPerspective(float fov, glm::vec2 z);
        glm::mat4 getPerspective();
        void update();
        float getDeltaTime();
        pair<double, double> updateCursor();
        pair<double, double> updateCursor(pair<double, double> pos);
        pair<double, double> getLastCursor();
        void setCursorPosCallback(GLFWcursorposfun callback);
        void setKeyCallback(GLFWkeyfun callback);
};

#endif // WINDOW_H_
