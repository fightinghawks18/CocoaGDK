#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 0) in vec2 inUv;
layout(location = 1) in vec4 inCol;

layout(std140) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;

out vec4 fragColor;
out vec2 fragUv;

void main() {
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(inPos, 1.0);
    fragColor = inCol;
    fragUv = inUv;
}