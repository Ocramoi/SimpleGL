#include "Object.hpp"

using std::make_shared;

void Object::setDefaultProgram(GLuint program) {
    defaultProgram = program;
}
auto Object::getDefaultProgram() -> decltype(defaultProgram) { return defaultProgram; }

void Object::updateCenter() {
    center = glm::vec3(0.f);
    for (const auto& el : elements)
        center += el->getCenter()/(1.f*elements.size());
}

void Object::popElement() {
    elements.pop_back();
}

auto Object::size() -> decltype(elements.size()) {
    return elements.size();
}

void Object::draw() {
    for (const auto& el : elements)
        el->draw();
}

void Object::setTransform(const glm::mat4& m) { transform = m; }
auto Object::getTransform() -> decltype(transform) { return transform; }

void Object::applyTransform() {
    applyTransform(defaultProgram);
}

void Object::applyTransform(GLuint target) {
    auto matTransf{glGetUniformLocation(target, "modelMatrix")};
    glUniformMatrix4fv(matTransf, 1, GL_TRUE, glm::value_ptr(transform));
}

void Object::applyTransform(const glm::mat4& m) {
    setTransform(m);
    applyTransform(defaultProgram);
}

void Object::applyTransform(const glm::mat4& m, GLuint target) {
    setTransform(m);
    applyTransform(target);
}

void Object::setColorFilter(const glm::mat4& m) { colorFilter = m; }
auto Object::getColorFilter() -> decltype(colorFilter) { return colorFilter; }

void Object::applyColorFilter() {
    applyColorFilter(defaultProgram);
}

void Object::applyColorFilter(GLuint target) {
    auto matColor{glGetUniformLocation(target, "colorMatrix")};
    glUniformMatrix4fv(matColor, 1, GL_TRUE, glm::value_ptr(colorFilter));
}

void Object::applyColorFilter(const glm::mat4& m) {
    setColorFilter(m);
    applyColorFilter(defaultProgram);
}

void Object::applyColorFilter(const glm::mat4& m, GLuint target) {
    setColorFilter(m);
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

    updateCenter();
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

    updateCenter();
}

void Object::pushElement(
    decltype(GL_TRIANGLES) drawType,
    const vector<glm::vec2>& points,
    const vector<glm::vec4>& colors,
    const vector<GLuint>& elementList
) {
    pushElement(drawType, points, colors, elementList, defaultProgram);
}

auto Object::getCenter() -> decltype(center) { return position + center; }

glm::vec3 Object::getPosition() { return position; }
void Object::setPosition(const glm::vec3 &pos) { position = pos; }
