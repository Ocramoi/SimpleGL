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
        vector<glm::vec3> points, 
                          normals;
        vector<glm::vec4> colors;
        vector<glm::vec2> uvs;
        vector<GLuint> elementList,
            textureIDs;
        vector<int> textureGroups;
        GLuint program,
            vao, cbo, vbo, uvo, no, ebo;
    public:
        Element(
            GLuint _program,
            decltype(GL_TRIANGLES) _drawType,
            const vector<glm::vec3>& _points,
            const vector<glm::vec3>& _normals,
            const vector<glm::vec2>& uvs,
            const vector<glm::vec4>& _colors,
            const vector<GLuint>& _elementList
        );
        ~Element();

        void draw();
        void bind();
        void unbind();
};

#endif // ELEMENT_H_
