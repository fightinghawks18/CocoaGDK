#include <stdio.h>

#include <glad/glad.h>

#include "opengl/opengl_context.h"
#include "opengl/opengl_ebo.h"
#include "opengl/opengl_loader.h"
#include "opengl/opengl_pipeline.h"
#include "opengl/opengl_shader.h"
#include "opengl/opengl_vao.h"
#include "platform/windowing.h"

int main() {
    if (ccoWindowingInit() != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize windowing!");
        return -1;
    }

    CcoWindow window = CCO_NULL_HANDLE;
    CcoResult windowResult =
        ccoCreateWindow(&(CcoWindowDesc){CCO_WINDOW_POS_CENTER, CCO_WINDOW_POS_CENTER, 800, 600, "cocoa",
                                         CCO_WINDOW_FLAG_DECOR_BIT | CCO_WINDOW_FLAG_RESIZE_BIT},
                        &window);
    if (windowResult != CCO_SUCCESS) {
        CCO_LOG("Failed to create window!");
        return -1;
    }

    CcoOpenGLContext glCtx = CCO_NULL_HANDLE;
    ccoCreateOpenGLContext(ccoGetNativeWindowHandle(window), NULL, &glCtx);
    ccoMakeCurrentOpenGLContext(glCtx);

    if (!gladLoadGLLoader(ccoGetGLProcAddr)) {
        CCO_LOG("Failed to load glad!");
        return -1;
    }

    CcoVertex vertices[3] = {
        {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    };

    u32 indices[3] = {
        0, 1, 2
    };

    CcoOpenGLVbo vbo = CCO_NULL_HANDLE;
    CcoOpenGLVao vao = CCO_NULL_HANDLE;
    CcoOpenGLEbo ebo = CCO_NULL_HANDLE;
    CcoOpenGLShader vs = CCO_NULL_HANDLE;
    CcoOpenGLShader ps = CCO_NULL_HANDLE;
    CcoOpenGLPipeline pip = CCO_NULL_HANDLE;

    ccoCreateOpenGLVbo(&vbo);
    ccoCreateOpenGLVao(&vao);
    ccoCreateOpenGLEbo(&ebo);
    ccoCreateOpenGLShader(&(CcoOpenGLShaderDesc){
        .shaderType = CCO_SHADER_TYPE_VERTEX,
        .shaderPath = "shaders/test.vert"
    }, &vs);
    ccoCreateOpenGLShader(&(CcoOpenGLShaderDesc){
        .shaderType = CCO_SHADER_TYPE_PIXEL,
        .shaderPath = "shaders/test.frag"
    }, &ps);
    ccoCreateOpenGLPipeline(&(CcoOpenGLPipelineDesc){
        .vertexShader = vs,
        .pixelShader = ps
    }, &pip);

    ccoMapToOpenGLVbo(vbo, &(CcoBufferMapping){
        .dataSize = 3 * sizeof(CcoVertex),
        .dataOffset = 0,
        .data = vertices
    });

    ccoMapToOpenGLEbo(ebo, &(CcoBufferMapping){
        .dataSize = 3 * sizeof(u32),
        .dataOffset = 0,
        .data = &indices
    });

    CcoVertexAttribute vertexAttributes[2] = {
        {
            .location = 0,
            .numComponents = 3,
            .stride = sizeof(CcoVertex),
            .offset = offsetof(CcoVertex, pos)
        },
        {
            .location = 1,
            .numComponents = 4,
            .stride = sizeof(CcoVertex),
            .offset = offsetof(CcoVertex, col)
        }
    };

    ccoSetOpenGLVaoLayout(vao, vbo, ebo, &(CcoVertexLayout){
        .attributes = vertexAttributes,
        .attributeCount = 2
    });

    while (!ccoShouldWindowClose(window)) {
        ccoWindowingPoll();

        glViewport(0, 0, 800, 600);
        glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ccoUseOpenGLPipeline(pip);
        ccoUseOpenGLVao(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        ccoFlushOpenGLContextBuffer(glCtx);
    }

    ccoCloseWindow(window);
    ccoWindowingQuit();
    return 0;
}