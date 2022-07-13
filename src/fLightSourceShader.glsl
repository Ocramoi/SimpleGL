#version 430

in vec4 _color;
in vec3 _position;
in vec2 Texcoord;

out vec4 outColor;

uniform mat4 colorMatrix;
layout (location = 4) uniform sampler2D tex;

void main(void) {
    outColor = 2 * texture(tex, Texcoord);
}
