#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texcoord;

out vec3 _position;
out vec4 _color;
out vec3 _fragPos;
out vec2 Texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void) {
    Texcoord = texcoord;
    _color = color;
    _position = position;
    _fragPos = vec3(modelMatrix * vec4(position, 1.0));
    gl_Position = modelMatrix * vec4(position, 1.0) * viewMatrix * projectionMatrix;
    // gl_Position = modelMatrix * vec4(position, 1.0);
}
