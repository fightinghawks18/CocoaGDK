#version 330 core

uniform sampler2D tex;
in vec4 fragColor;
in vec2 fragUv;
out vec4 outColor;

void main() {
    outColor = texture(tex, fragUv) * fragColor;
}