#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "../utils/util.hpp"

using std::vector;

class Element {
    private:
        decltype(GL_TRIANGLES) drawType;
        vector<Utils::vec3> points;
        vector<Utils::vec4> colors;
        vector<GLuint> elementList;
        GLuint program,
            vao, cbo, vbo, ebo;
    public:
        Element(
            GLuint _program,
            decltype(GL_TRIANGLES) _drawType,
            const vector<Utils::vec3>& _points,
            const vector<Utils::vec4>& _colors,
            const vector<GLuint>& _elementList
        );
        ~Element();

        void draw();
        void bind();
        void unbind();
};

#endif // ELEMENT_H_
