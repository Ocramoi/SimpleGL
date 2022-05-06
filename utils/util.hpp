#ifndef UTIL_H_
#define UTIL_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <algorithm>

#include "../classes/Transform.hpp"

using std::unique_ptr;
using std::pair;
using std::string;

namespace Utils {
    using vec2=struct {
        GLfloat x, y;
    };

    using vec3=struct {
        GLfloat x, y, z;
    };

    using vec4=struct {
        GLfloat x, y, z, h;
    };

    GLuint createRenderProgram(string vShader, string fShader);
    string readFileContent(string path);
    void unsetModel(GLuint program, glm::mat4 Model = glm::Identity4);
    void setView(GLuint program, glm::mat4 Model = glm::Identity4);
    void setProjection(GLuint program, float fov, glm::vec2 size, glm::vec2 z);
    void setProjection(GLuint program, glm::mat4 Model = glm::Identity4);

    void deletePrograms(GLuint program);
    template <class ...P>
    void deletePrograms(GLuint target, P... next) {
        deletePrograms(target);
        deletePrograms(next...);
    };
};

#endif // UTIL_H_
