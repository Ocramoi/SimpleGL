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

bool Utils::loadObjectFromFile(const char* path, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals){
    vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    vector<glm::vec3> tempVertices;
    vector<glm::vec2> tempUvs;
    vector<glm::vec3> tempNormals;

    bool firstGroup = true;
    int numberOfVertices = 0;

    FILE* file = fopen(path, "r");
    if (file == NULL){
        return false;
    } 

    while (true) {
        char lineHeader[512];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;

        // Reads vertices, textures, normals, faces and texture groups
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            tempVertices.push_back(vertex);
        } else if ( strcmp( lineHeader, "vt" ) == 0 ) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            tempUvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            tempNormals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            
            long int before_position = ftell(file);
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            
            // If doesn't have normal
            if (matches != 9){ 
                fseek(file, before_position, SEEK_SET);
                fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
            }

            // Auxiliary vectors
            vertexIndices.push_back(vertexIndex[0]); 
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        } else {
            fscanf(file, "[^\n]");
        }
    }

    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = tempVertices[vertexIndex - 1];
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = tempUvs[uvIndex - 1];
        uvs.push_back(uv);
    }

    for (unsigned int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = tempNormals[normalIndex - 1];
        normals.push_back(normal);
    }

    fclose(file);
    return true;
}
