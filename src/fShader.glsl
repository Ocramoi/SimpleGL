#version 430

in vec4 _color;
in vec2 Texcoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

uniform mat4 colorMatrix;
layout (location = 4) uniform sampler2D tex;
layout (location = 5) uniform vec3 lightPos;
layout (location = 6) uniform vec3 lightColor;

void main(void) {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float intensity = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = vec4(intensity * lightColor, 1.0);
    outColor = colorMatrix * diffuse * _color * texture(tex, Texcoord);
}
