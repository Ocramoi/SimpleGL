#ifndef OBJECT_H_
#define OBJECT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <memory>

#include "./Element.hpp"
#include "../utils/util.hpp"

using std::vector;
using std::shared_ptr;

class Object {
    private:
        GLuint defaultProgram;
        vector<shared_ptr<Element>> elements;
        glm::vec3 pos{0, 0, 0};
        glm::mat4 transform{1.f},
                colorFilter{1.f};
    public:
        Object(GLuint program) : defaultProgram(program) {};
        void setDefaultProgram(GLuint program);
        decltype(defaultProgram) getDefaultProgram();

        void pushElement(
            decltype(GL_TRIANGLES) drawType,
            const vector<glm::vec3>& points,
            const vector<glm::vec4>& colors,
            const vector<GLuint>& elementList
        );
        void pushElement(
            decltype(GL_TRIANGLES) drawType,
            const vector<glm::vec3>& points,
            const vector<glm::vec4>& colors,
            const vector<GLuint>& elementList,
            GLuint program
        );

        void pushElement(
            decltype(GL_TRIANGLES) drawType,
            const vector<glm::vec2>& points,
            const vector<glm::vec4>& colors,
            const vector<GLuint>& elementList
        );
        void pushElement(
            decltype(GL_TRIANGLES) drawType,
            const vector<glm::vec2>& points,
            const vector<glm::vec4>& colors,
            const vector<GLuint>& elementList,
            GLuint program
        );

        void popElement();
        size_t size();

        void setTransform(glm::mat4 m);
        glm::mat4 getTransform();
        void applyTransform();
        void applyTransform(GLuint target);

        void setColorFilter(glm::mat4 m);
        glm::mat4 getColorFilter();
        void applyColorFilter();
        void applyColorFilter(GLuint target);

        void draw();
};

#endif // OBJECT_H_
