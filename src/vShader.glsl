#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
out vec3 _position;
out vec4 _color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void) {
    _color = color;
    _position = position;
    gl_Position = modelMatrix * vec4(position, 1.0) * viewMatrix * projectionMatrix;
    // gl_Position = modelMatrix * vec4(position, 1.0);
}
