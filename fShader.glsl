#version 430

in vec4 _color;
out vec4 outColor;

in vec3 _position;

uniform mat4 colorMatrix;
uniform vec4 center;

void main(void) {
    outColor = colorMatrix * _color;
}
