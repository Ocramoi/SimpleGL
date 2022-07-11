#version 430

in vec4 _color;
in vec3 _position;
in vec2 Texcoord;

out vec4 outColor;

uniform mat4 colorMatrix;
layout (location = 3) uniform sampler2D tex;

void main(void) {
    outColor = texture(tex, Texcoord) * _color;
}
