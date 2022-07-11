#ifndef UTIL_H_
#define UTIL_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <algorithm>

#include "../classes/Transform.hpp"
#include <GL/glew.h>
#include <GL/gl.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 
#include <math.h>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

using std::unique_ptr;
using std::pair;
using std::string;

namespace Utils {    
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
    
    bool loadObjectFromFile(const char* path, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<int>& textureGroups);
};

#endif // UTIL_H_
