#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 normal;

out vec4 _color;
out vec3 FragPos;
out vec2 Texcoord;
out vec3 Normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void) {
    Texcoord = texcoord;
    Normal = normal;
    _color = color;
    FragPos = vec3(modelMatrix * vec4(position, 1.0));
    gl_Position = modelMatrix * vec4(position, 1.0) * viewMatrix * projectionMatrix;
    // gl_Position = modelMatrix * vec4(position, 1.0);
}
