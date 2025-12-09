#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "opengl/shader.h"
#include "opengl/program.h"
#include "opengl/vao.h"

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize glfw!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(800, 600, "cocoa", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to load GL with glad!\n");
        return -1;
    }

    CcoGLVertexArrayObject vertexArrayObject = ccoCreateGLVertexArrayObject();

    CcoGLShader vertexShader = CCO_NULL_HANDLE;
    if (ccoCreateGLShader(
        &(CcoGLShaderDesc){ .shaderType = CCO_SHADER_TYPE_VERTEX, .shaderPath = "shaders/test.vert" },
        &vertexShader) != CCO_SUCCESS) {
        printf("Failed to create vertex shader!\n");
        return -1;
    }

    CcoGLShader pixelShader = CCO_NULL_HANDLE;
    if (ccoCreateGLShader(
        &(CcoGLShaderDesc){ .shaderType = CCO_SHADER_TYPE_PIXEL, .shaderPath = "shaders/test.frag" },
        &pixelShader) != CCO_SUCCESS) {
        printf("Failed to create fragment shader!\n");
        return -1;
    }

    CcoGLProgram program = CCO_NULL_HANDLE;
    if (ccoCreateGLProgram(
        &(CcoGLProgramDesc){ .vertexShader = vertexShader, .pixelShader = pixelShader },
        &program) != CCO_SUCCESS) {
        printf("Failed to create program!\n");
        return -1;
    }

    glBindVertexArray(ccoGetGLVertexArrayObjectId(vertexArrayObject));

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        glViewport(0, 0, w, h);

        glClearColor(0.35f, 0.15f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ccoGetGLProgramId(program));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}