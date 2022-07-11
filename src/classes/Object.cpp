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

    updateCenter();
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

    updateCenter();
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

auto Object::getCenter() -> decltype(center) { return position + center; }

glm::vec3 Object::getPosition() { return position; }
void Object::setPosition(const glm::vec3 &pos) { position = pos; }
 
void Object::applyTexture (std::vector<GLuint> &textureIDs) {
    SDL_Surface *image;
    for (unsigned int i = 0; i < textures.size(); i++){ 
        
        image = IMG_Load(textures[i].texturePath.c_str()); 
        if (image == NULL){
            throw("Unable to get a texture from the file");
        }

        glGenTextures(1, &textureIDs[i]);
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, textures[i].format, GL_UNSIGNED_BYTE, image->pixels);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}