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
        float FOV{60.f}, zMin{0.01f}, zMax{1000.f};
        glm::mat4 perspective{1.f};
        pair<float, float> wSize;
    public:
        static void initContext(size_t samples);
        static void clear();
        static void clear(array<float, 4> Color);
        GLFWwindow* get();
        Window(
            const pair<float, float>& size,
            const string& title,
            GLFWmonitor *monitor,
            GLFWwindow *share
        );
        ~Window();
        void setPerspective(glm::mat4 Perspective = glm::Identity4);
        void setPerspective(float fov, glm::vec2 z);
        glm::mat4 getPerspective();
};

#endif // WINDOW_H_
