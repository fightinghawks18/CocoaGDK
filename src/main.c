#include "opengl/ebo.h"


#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "opengl/shader.h"
#include "opengl/program.h"
#include "opengl/vao.h"
#include "opengl/vbo.h"

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

    CcoGLVertexLayoutDesc posLayout = {
        0,
        GL_FLOAT,
        3,
        sizeof(CcoVertex),
        offsetof(CcoVertex, pos)
    };

    CcoGLVertexLayoutDesc colLayout = {
        1,
        GL_FLOAT,
        4,
        sizeof(CcoVertex),
        offsetof(CcoVertex, col)
    };

    CcoGLVertexLayoutDesc layouts[2] = {posLayout, colLayout};
    CcoGLVertexArrayObjectDesc vertexArrayDesc = {
        .layouts = layouts,
        .layoutCount = 2
    };

    CcoGLVertexBufferObject vertexBufferObject = ccoCreateGLVertexBufferObject();
    CcoGLElementBufferObject elementBufferObject = ccoCreateGLElementBufferObject();
    ccoUseGLVertexBufferObject(vertexBufferObject);

    CcoGLVertexArrayObject vertexArrayObject = ccoCreateGLVertexArrayObject(&vertexArrayDesc);

    CcoVertex vertices[] = {
        {{-0.5f, -0.5f, 0.0f}, {1, 0, 0, 1}},
        {{0.5f, -0.5f, 0.0f}, {0, 1, 0, 1}},
        {{0.0f, 0.5f, 0.0f}, {0, 0, 1, 1}}
    };

    u32 indices[] = {
        0, 1, 2
    };

    ccoMapGLVertexBufferObject(vertexBufferObject, &(CcoBufferMapper){ .offset = 0, .size = 3 * sizeof(CcoVertex), .data = &vertices });
    ccoMapGLElementBufferObject(elementBufferObject, &(CcoBufferMapper){ .offset = 0, .size = 3 * sizeof(u32), .data = &indices });
    ccoUseGLElementBufferObject(elementBufferObject);

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

    ccoUseGLVertexArrayObject(vertexArrayObject);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        glViewport(0, 0, w, h);

        glClearColor(0.35f, 0.15f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ccoUseGLProgram(program);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}