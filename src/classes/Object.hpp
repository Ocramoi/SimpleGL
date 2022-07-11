#ifndef OBJECT_H_
#define OBJECT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include <vector>
#include <memory>

#include "./Element.hpp"
#include "../utils/util.hpp"

using std::vector;
using std::shared_ptr;

typedef struct _textureInfo {
    std::string texturePath;
    GLenum format;
} TextureInfo;

class Object {
    private:
        GLuint defaultProgram;
        vector<shared_ptr<Element>> elements;
        glm::vec3 position{0.f},
                center{0.f};
        glm::mat4 transform{1.f},
                colorFilter{1.f};
        bool textured{false};
        GLuint textureId;

        void updateCenter();
    public:
        Object(GLuint program) : defaultProgram(program) {};
        void setDefaultProgram(GLuint program);
        decltype(defaultProgram) getDefaultProgram();
        
        void pushElement(
            decltype(GL_TRIANGLES) drawType,
            const vector<glm::vec3>& points,
            const vector<glm::vec3>& normals,
            const vector<glm::vec2>& uvs,
            const vector<glm::vec4>& colors,
            const vector<GLuint>& elementList
        );
        void pushElement(
            decltype(GL_TRIANGLES) drawType,
            const vector<glm::vec3>& points,
            const vector<glm::vec3>& normals,
            const vector<glm::vec2>& uvs,
            const vector<glm::vec4>& colors,
            const vector<GLuint>& elementList,
            GLuint program
        );
        void pushElement(
            decltype(GL_TRIANGLES) drawType,
            const vector<glm::vec2>& points,
            const vector<glm::vec3>& normals,
            const vector<glm::vec2>& uvs,
            const vector<glm::vec4>& colors,
            const vector<GLuint>& elementList
        );
        void pushElement(
            decltype(GL_TRIANGLES) drawType,
            const vector<glm::vec2>& points,
            const vector<glm::vec3>& normals,
            const vector<glm::vec2>& uvs,
            const vector<glm::vec4>& colors,
            const vector<GLuint>& elementList,
            GLuint program
        );

        void popElement();
        size_t size();

        void setTransform(const glm::mat4& m);
        glm::mat4 getTransform();
        void applyTransform();
        void applyTransform(GLuint target);
        void applyTransform(const glm::mat4& m);
        void applyTransform(const glm::mat4& m, GLuint target);

        void setColorFilter(const glm::mat4& m);
        glm::mat4 getColorFilter();
        void applyColorFilter();
        void applyColorFilter(GLuint target);
        void applyColorFilter(const glm::mat4& m);
        void applyColorFilter(const glm::mat4& m, GLuint target);

        bool loadFromFile(const string& objPath, const string& texturePath);
        // void applyTexture(std::vector<GLuint> &textureIDs);

        glm::vec3 getPosition();
        void setPosition(const glm::vec3& pos);

        void draw();
};

#endif // OBJECT_H_
