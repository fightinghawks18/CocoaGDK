#include <stdio.h>

#include <glad/glad.h>

#include "opengl/opengl_context.h"
#include "opengl/opengl_ebo.h"
#include "opengl/opengl_loader.h"
#include "opengl/opengl_pipeline.h"
#include "opengl/opengl_shader.h"
#include "opengl/opengl_ubo.h"
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
        {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    };

    u32 indices[3] = {0, 1, 2};

    CcoVector3 position = ccoCreateVector3(0, 0, 0);
    CcoVector3 rotation = ccoCreateVector3(0, 0, 0);
    CcoVector3 scale = ccoCreateVector3(1, 1, 1);

    CcoVector3 cameraPosition = ccoCreateVector3(0, 0, 12.0f);

    CcoMatrix4X4 modelMatrix = ccoCreateModelMatrix4X4(
        ccoCreateTranslationMatrix4X4(position), ccoCreateRotationMatrix4X4(rotation), ccoCreateScaleMatrix4x4(scale));
    CcoMatrix4X4 viewMatrix = ccoCreateEyeMatrix4X4(cameraPosition, ccoCreateVector3(0, 0, 0), ccoCreateVector3Up());
    CcoMatrix4X4 projectionMatrix =
        ccoCreatePerspectiveMatrix4X4(ccoDegreesToRadian(80.0f), 800.0f / 600.0f, 0.001f, 100.0f);

    CcoModelViewProjection mvpBuffer = {.model = ccoTransposeMatrix4X4(modelMatrix),
                                        .view = ccoTransposeMatrix4X4(viewMatrix),
                                        .projection = ccoTransposeMatrix4X4(projectionMatrix)};

    CcoOpenGLVbo vbo = CCO_NULL_HANDLE;
    CcoOpenGLVao vao = CCO_NULL_HANDLE;
    CcoOpenGLEbo ebo = CCO_NULL_HANDLE;
    CcoOpenGLUbo ubo = CCO_NULL_HANDLE;
    CcoOpenGLShader vs = CCO_NULL_HANDLE;
    CcoOpenGLShader ps = CCO_NULL_HANDLE;
    CcoOpenGLPipeline pip = CCO_NULL_HANDLE;

    ccoCreateOpenGLVbo(&vbo);
    ccoCreateOpenGLVao(&vao);
    ccoCreateOpenGLEbo(&ebo);
    ccoCreateOpenGLUbo(&ubo);

    ccoCreateOpenGLShader(
        &(CcoOpenGLShaderDesc){.shaderType = CCO_SHADER_TYPE_VERTEX, .shaderPath = "shaders/test.vert"}, &vs);
    ccoCreateOpenGLShader(
        &(CcoOpenGLShaderDesc){.shaderType = CCO_SHADER_TYPE_PIXEL, .shaderPath = "shaders/test.frag"}, &ps);
    ccoCreateOpenGLPipeline(&(CcoOpenGLPipelineDesc){.vertexShader = vs, .pixelShader = ps}, &pip);

    ccoMapToOpenGLVbo(vbo, &(CcoBufferMapping){.dataSize = 3 * sizeof(CcoVertex), .dataOffset = 0, .data = vertices});

    ccoMapToOpenGLEbo(ebo, &(CcoBufferMapping){.dataSize = 3 * sizeof(u32), .dataOffset = 0, .data = indices});

    ccoMapToOpenGLUbo(
        ubo, &(CcoBufferMapping){.dataSize = sizeof(CcoModelViewProjection), .dataOffset = 0, .data = &mvpBuffer});

    CcoVertexAttribute vertexAttributes[2] = {
        {.location = 0, .numComponents = 3, .stride = sizeof(CcoVertex), .offset = offsetof(CcoVertex, pos)},
        {.location = 1, .numComponents = 4, .stride = sizeof(CcoVertex), .offset = offsetof(CcoVertex, col)}};

    ccoSetOpenGLVaoLayout(vao, vbo, ebo, &(CcoVertexLayout){.attributes = vertexAttributes, .attributeCount = 2});

    while (!ccoShouldWindowClose(window)) {
        ccoWindowingPoll();

        CcoWindowDimensions windowDimensions = ccoGetWindowDimensions(window);

        projectionMatrix = ccoCreatePerspectiveMatrix4X4(
            ccoDegreesToRadian(80.0f), (f32)windowDimensions.w / (f32)windowDimensions.h, 0.001f, 100.0f);
        mvpBuffer.projection = ccoTransposeMatrix4X4(projectionMatrix);
        ccoMapToOpenGLUbo(ubo, &(CcoBufferMapping){.dataSize = sizeof(CcoModelViewProjection),
                                                   .dataOffset = offsetof(CcoModelViewProjection, projection),
                                                   .data = &mvpBuffer.projection});

        glViewport(0, 0, windowDimensions.w, windowDimensions.h);
        glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        ccoUseOpenGLPipeline(pip);
        ccoUseOpenGLVao(vao);
        ccoUseOpenGLUbo(&(CcoOpenGLUboBinding){.type = CCO_OPENGL_UBO_BINDING_BLOCK_NAME, .pip = pip, .name = "MVP"},
                        ubo);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

        ccoFlushOpenGLContextBuffer(glCtx);
    }

    ccoCloseWindow(window);
    ccoWindowingQuit();
    return 0;
}