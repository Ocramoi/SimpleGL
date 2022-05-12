#include "Window.hpp"

void Window::initContext(size_t samples) {
    if (!glfwInit()) {
        cerr << "Unable to initialize GLFW3!" << endl;
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);

    if (samples) {
        glfwWindowHint(GLFW_SAMPLES, samples);
        glEnable(GL_MULTISAMPLE);
    }
}

Window::Window(
    const pair<float, float>& size,
    const string& title,
    GLFWmonitor *monitor,
    GLFWwindow *share,
    bool context3d
) {
    _win = glfwCreateWindow(
        size.first, size.second,
        title.c_str(),
        monitor, share
    );
    if (!_win) {
        cerr << "Unable to create GLFW window!" << endl;
        exit(1);
    }
    wSize = size;

    enable3d = context3d;
    glfwMakeContextCurrent(_win);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Unable to initialize GLEW!" << endl;
        exit(1);
    }
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    const GLubyte *renderer = glGetString(GL_RENDERER),
        *version = glGetString(GL_VERSION);
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported: " << version << endl;

    setPerspective(FOV, { zMin, zMax });
}

Window::~Window() {
    glfwDestroyWindow(_win);
    glfwTerminate();
}

auto Window::get() -> decltype(_win) { return _win; }

void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT | (enable3d ? GL_DEPTH_BUFFER_BIT : 0));
}

void Window::clear(array<float, 4> Color) {
    glClearColor(Color[0], Color[1], Color[2], Color[3]);
    clear();
}

void Window::setPerspective(glm::mat4 Perspective) { perspective = Perspective; }
auto Window::getPerspective() -> decltype(perspective) { return perspective; }

void Window::setPerspective(float fov, glm::vec2 z) {
    FOV = fov; zMin = z.x; zMax = z.y;
    setPerspective(glm::perspective(glm::radians(FOV), wSize.first/wSize.second, zMin, zMax));
}

auto Window::getDeltaTime() -> decltype(deltaTime) { return deltaTime; }

void Window::update() {
    glfwSwapBuffers(get());
    auto curFrame{glfwGetTime()};
    deltaTime = curFrame - lastFrame;
    lastFrame = curFrame;
}
