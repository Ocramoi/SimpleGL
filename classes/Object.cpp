#include "Object.hpp"

using std::make_shared;

void Object::setDefaultProgram(GLuint program) {
    defaultProgram = program;
}
GLuint Object::getDefaultProgram() { return defaultProgram; }

void Object::popElement() {
    elements.pop_back();
}

size_t Object::size() {
    return elements.size();
}

void Object::draw() {
    for (const auto& el : elements)
        el->draw();
}

void Object::setTransform(glm::mat4 m) { transform = m; }
glm::mat4 Object::getTransform() { return transform; }

void Object::applyTransform() {
    Object::applyTransform(defaultProgram);
}

void Object::applyTransform(GLuint target) {
    auto matTransf{glGetUniformLocation(target, "modelMatrix")};
    glUniformMatrix4fv(matTransf, 1, GL_TRUE, glm::value_ptr(transform));
}

void Object::setColorFilter(glm::mat4 m) { colorFilter = m; }
glm::mat4 Object::getColorFilter() { return colorFilter; }

void Object::applyColorFilter() {
    Object::applyColorFilter(defaultProgram);
}

void Object::applyColorFilter(GLuint target) {
    auto matColor{glGetUniformLocation(target, "colorMatrix")};
    glUniformMatrix4fv(matColor, 1, GL_TRUE, glm::value_ptr(colorFilter));
}

void Object::pushElement(
    decltype(GL_TRIANGLES) drawType,
    const vector<glm::vec3>& points,
    const vector<glm::vec4>& colors,
    const vector<GLuint>& elementList,
    GLuint program
) {
    if (elementList.empty()) {
        vector<GLuint> sequence;
        for (GLuint i{0}; i < points.size(); ++i) sequence.push_back(i);
        elements.push_back(make_shared<Element>(
            program,
            drawType,
            points,
            colors,
            sequence
        ));
    } else {
        elements.push_back(make_shared<Element>(
            program,
            drawType,
            points,
            colors,
            elementList
        ));
    }
}

void Object::pushElement(
    decltype(GL_TRIANGLES) drawType,
    const vector<glm::vec3>& points,
    const vector<glm::vec4>& colors,
    const vector<GLuint>& elementList
) {
    pushElement(drawType, points, colors, elementList, defaultProgram);
}

void Object::pushElement(
    decltype(GL_TRIANGLES) drawType,
    const vector<glm::vec2>& points,
    const vector<glm::vec4>& colors,
    const vector<GLuint>& elementList,
    GLuint program
) {
    vector<glm::vec3> points3d;
    for (const auto& point : points) points3d.push_back({point.x, point.y, 0.f});

    if (elementList.empty()) {
        vector<GLuint> sequence;
        for (GLuint i{0}; i < points.size(); ++i) sequence.push_back(i);
        elements.push_back(make_shared<Element>(
            program,
            drawType,
            points3d,
            colors,
            sequence
        ));
    } else {
        elements.push_back(make_shared<Element>(
            program,
            drawType,
            points3d,
            colors,
            elementList
        ));
    }
}

void Object::pushElement(
    decltype(GL_TRIANGLES) drawType,
    const vector<glm::vec2>& points,
    const vector<glm::vec4>& colors,
    const vector<GLuint>& elementList
) {
    pushElement(drawType, points, colors, elementList, defaultProgram);
}
