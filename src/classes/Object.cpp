#include "Object.hpp"

using std::make_shared;

void Object::setDefaultProgram(GLuint program) {
    defaultProgram = program;
}
auto Object::getDefaultProgram() -> decltype(defaultProgram) { return defaultProgram; }

void Object::popElement() {
    elements.pop_back();
}

auto Object::size() -> decltype(elements.size()) {
    return elements.size();
}

void Object::draw() {
    if (textured) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(4, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    for (const auto& el : elements)
        el->draw();
}

#include <iostream>

void Object::setTransform(const glm::mat4& m) {
    transform = m;
    glm::vec4 toCenter{0, 0, 0, 1};
    toCenter = glm::transpose(transform) * toCenter;
    normCenter = { toCenter.x, toCenter.y, toCenter.z };
}
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
    const vector<glm::vec3>& normals,
    const vector<glm::vec2>& uvs,
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
            normals,
            uvs,
            colors,
            sequence
        ));
    } else {
        elements.push_back(make_shared<Element>(
            program,
            drawType,
            points,
            normals,
            uvs,
            colors,
            elementList
        ));
    }
}

void Object::pushElement(
    decltype(GL_TRIANGLES) drawType,
    const vector<glm::vec3>& points,
    const vector<glm::vec3>& normals,
    const vector<glm::vec2>& uvs,
    const vector<glm::vec4>& colors,
    const vector<GLuint>& elementList
) {
    pushElement(drawType, points, normals, uvs, colors, elementList, defaultProgram);
}

void Object::pushElement(
    decltype(GL_TRIANGLES) drawType,
    const vector<glm::vec2>& points,
    const vector<glm::vec3>& normals,
    const vector<glm::vec2>& uvs,
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
            normals,
            uvs,
            colors,
            sequence
        ));
    } else {
        elements.push_back(make_shared<Element>(
            program,
            drawType,
            points3d,
            normals,
            uvs,
            colors,
            elementList
        ));
    }
}

void Object::pushElement(
    decltype(GL_TRIANGLES) drawType,
    const vector<glm::vec2>& points,
    const vector<glm::vec3>& normals,
    const vector<glm::vec2>& uvs,
    const vector<glm::vec4>& colors,
    const vector<GLuint>& elementList
) {
    pushElement(drawType, points, normals, uvs, colors, elementList, defaultProgram);
}

auto Object::getPosition() -> decltype(getPosition()) { return position; }
void Object::setPosition(const glm::vec3 &pos) { position = pos; }

auto Object::loadFromFile(
    const string &objPath,
    const string &texturePath
) -> decltype(loadFromFile(objPath, texturePath)) {
    vector<glm::vec3> vertices,
        normals;
    vector<glm::vec2> uvs;
    if (!Utils::loadObjectFromFile(objPath.c_str(), vertices, uvs, normals))
        return false;

    int w, h, c;
    unsigned char* image = SOIL_load_image(texturePath.c_str(), &w, &h, &c, SOIL_LOAD_RGB);
    if (image == NULL) throw("Unable to get a texture from the file");
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);
    textured = true;

    vector<glm::vec4> tmpColors{vertices.size(), {1, 1, 1, 1}};
    pushElement(GL_TRIANGLE_STRIP, vertices, normals, uvs, tmpColors, {});
    SOIL_free_image_data(image);

    return true;
}

void Object::setupDefaultRenderer(const unique_ptr<Camera> &camera, Window& win) {
    glUseProgram(getDefaultProgram());
    Utils::setProjection(getDefaultProgram(), win.getPerspective());
    camera->setView(getDefaultProgram());
}

void Object::emitLight(const glm::vec3 &lightColor) {
    glUniform3fv(5, 1, glm::value_ptr(normCenter));
    glUniform3fv(6, 1, glm::value_ptr(lightColor));
}
