#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inCol;

layout(std140) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;

out vec4 fragColor;

void main() {
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(inPos, 1.0);
    fragColor = inCol;
}