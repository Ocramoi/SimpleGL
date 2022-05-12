#include "util.hpp"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

using std::exit;
using std::cout;
using std::cerr;
using std::endl;

using std::ifstream;
using std::ios;

using std::vector;

auto Utils::readFileContent(string path) -> decltype(path) {
    ifstream fStream{path, ios::in | ios::binary}; fStream.seekg(0, fStream.end);
    vector<char> rawContent(static_cast<long>(fStream.tellg()) + 1); fStream.seekg(0, fStream.beg);
    fStream.read(&rawContent[0], rawContent.size() - 1); rawContent.back() = '\0';
    fStream.close();

    return string(rawContent.begin(), rawContent.end());
}

auto createShader(string vPath, string fPath) -> GLuint {
    auto vShaderSrc = Utils::readFileContent(vPath),
        fShaderSrc = Utils::readFileContent(fPath);

    GLuint vShader{glCreateShader(GL_VERTEX_SHADER)},
        fShader{glCreateShader(GL_FRAGMENT_SHADER)},
        vfProgram{glCreateProgram()};

    {
        const char *_vShaderSrc{vShaderSrc.c_str()},
            *_fShaderSrc{fShaderSrc.c_str()};
        glShaderSource(vShader, 1, &_vShaderSrc, NULL);
        glShaderSource(fShader, 1, &_fShaderSrc, NULL);

        GLint isCompiled{0};

        glCompileShader(vShader);
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint msgLen; glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &msgLen);
            char errMsg[msgLen]; glGetShaderInfoLog(vShader, msgLen, NULL, errMsg);
            cerr << "Error while compiling vertex shader " << vPath << endl << errMsg << endl;
        }

        glCompileShader(fShader);
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint msgLen; glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &msgLen);
            char errMsg[msgLen]; glGetShaderInfoLog(fShader, msgLen, NULL, errMsg);
            cerr << "Error while compiling fragment shader " << fPath << endl << errMsg << endl;
        }

        glAttachShader(vfProgram, vShader);
        glAttachShader(vfProgram, fShader);
        glLinkProgram(vfProgram);

        glBindFragDataLocation(vfProgram, 0, "outColor");
    }

    glUseProgram(vfProgram);
    glDeleteShader(vShader); glDeleteShader(fShader);

    return vfProgram;
}

auto Utils::createRenderProgram(string vShader, string fShader) -> GLuint {
    GLuint program = createShader(vShader, fShader);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        char errLog[1024]; glGetProgramInfoLog(program, 1024, NULL, errLog);
        cerr << "Linking " << errLog << endl;
        exit(1);
    }

    return program;
}

void Utils::unsetModel(GLuint program, glm::mat4 Model) {
    auto mTransf{glGetUniformLocation(program, "modelMatrix")};
    glUniformMatrix4fv(mTransf, 1, GL_TRUE, glm::value_ptr(Model));
}

void Utils::setProjection(
    GLuint program,
    float fov,
    glm::vec2 size,
    glm::vec2 z
){
    setProjection(program, glm::perspectiveFov(fov, size.x, size.y, z.x, z.y));
}

void Utils::setProjection(GLuint program, glm::mat4 Model){
    auto pTransf{glGetUniformLocation(program, "projectionMatrix")};
    glUniformMatrix4fv(pTransf, 1, GL_TRUE, glm::value_ptr(Model));
}

void Utils::deletePrograms(GLuint program) { glDeleteProgram(program); }

void Utils::setView(GLuint program, glm::mat4 Model) {
    auto vTransf{glGetUniformLocation(program, "viewMatrix")};
    glUniformMatrix4fv(vTransf, 1, GL_TRUE, glm::value_ptr(Model));
}
