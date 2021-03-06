#include "Element.hpp"

Element::Element(
    GLuint _program,
    decltype(GL_TRIANGLES) _drawType,
    const vector<glm::vec3>& _points,
    const vector<glm::vec4>& _colors,
    const vector<GLuint>& _elementList
) {
    program = _program;
    drawType = _drawType;
    points = _points; colors = _colors; elementList = _elementList;

    for (const auto& point : points) center += point/(1.f*points.size());

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &cbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        points.size() * sizeof(decltype(points[0])),
        points.data(),
        GL_DYNAMIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        colors.size() * sizeof(decltype(colors[0])),
        colors.data(),
        GL_DYNAMIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        elementList.size() * sizeof(decltype(elementList[0])),
        elementList.data(),
        GL_DYNAMIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3*sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, 4*sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Element::~Element() {
    glDeleteBuffers(1, &vbo); glDeleteBuffers(1, &cbo); glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Element::bind() {
    glUseProgram(program);
    auto pTransf{glGetUniformLocation(program, "center")};
    glUniformMatrix4fv(pTransf, 1, GL_TRUE, glm::value_ptr(center));
    glBindVertexArray(vao);
}

void Element::unbind() {
    glBindVertexArray(0);
}

void Element::draw() {
    bind();
    glDrawElements(drawType, elementList.size(), GL_UNSIGNED_INT, nullptr);
    unbind();
}
