#include <stdio.h>

#include "opengl/opengl_context.h"
#include "opengl/opengl_core.h"
#include "opengl/opengl_ebo.h"
#include "opengl/opengl_pipeline.h"
#include "opengl/opengl_shader.h"
#include "opengl/opengl_ubo.h"
#include "opengl/opengl_vao.h"
#include "platform/utils.h"
#include "platform/windowing.h"

int main() {
    if (ccoWindowingInit() != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize windowing!");
        return -1;
    }

    CcoWindow window = CCO_NIL;
    CcoResult windowResult =
        ccoCreateWindow(&(CcoWindowDesc){CCO_WINDOW_POS_CENTER, CCO_WINDOW_POS_CENTER, 800, 600, "cocoa",
                                         CCO_WINDOW_FLAG_DECOR_BIT | CCO_WINDOW_FLAG_RESIZE_BIT},
                        &window);
    if (windowResult != CCO_SUCCESS) {
        CCO_LOG("Failed to create window!");
        return -1;
    }

    CcoOpenGLContext glCtx = CCO_NIL;
    ccoCreateOpenGLContext(ccoGetNativeWindowHandle(window), NULL, &glCtx);
    ccoMakeCurrentOpenGLContext(glCtx);

    ccoInitializeOpenGL();

    CcoVertex vertices[3] = {
        {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    };

    u32 indices[3] = {0, 1, 2};

    Vec3 position = ccoVec3(0, 0, 0);
    Vec3 rotation = ccoVec3(0, 0, 0);
    Vec3 scale = ccoVec3(1, 1, 1);

    Vec3 cameraPosition = ccoVec3(0, 0, 1.0f);

    Mat4 modelMatrix = ccoCreateModelMat4(
        ccoCreateTranslationMat4(position), ccoCreateRotationMat4(rotation), ccoCreateScaleMatrix4x4(scale));
    Mat4 viewMatrix = ccoCreateEyeMat4(cameraPosition, ccoVec3(0, 0, 0), ccoVec3Up());
    Mat4 projectionMatrix =
        ccoCreatePerspectiveMat4(CCO_NO, CCO_NO, ccoDegreesToRadian(80.0f), 800.0f / 600.0f, 0.001f, 100.0f);

    CcoModelViewProjection mvpBuffer = {.model = ccoTransposeMat4(modelMatrix),
                                        .view = ccoTransposeMat4(viewMatrix),
                                        .projection = ccoTransposeMat4(projectionMatrix)};

    CcoOpenGLVbo vbo = CCO_NIL;
    CcoOpenGLVao vao = CCO_NIL;
    CcoOpenGLEbo ebo = CCO_NIL;
    CcoOpenGLUbo ubo = CCO_NIL;
    CcoOpenGLShader vs = CCO_NIL;
    CcoOpenGLShader ps = CCO_NIL;
    CcoOpenGLPipeline pip = CCO_NIL;

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

        ccoMakeCurrentOpenGLContext(glCtx);

        CcoWindowFramebufferSize windowFramebufferSize = ccoGetWindowFramebufferSize(window);

        projectionMatrix =
            ccoCreatePerspectiveMat4(CCO_NO, CCO_NO, ccoDegreesToRadian(80.0f),
                                          (f32)windowFramebufferSize.w / (f32)windowFramebufferSize.h, 0.001f, 100.0f);
        mvpBuffer.projection = ccoTransposeMat4(projectionMatrix);

        ccoMapToOpenGLUbo(ubo, &(CcoBufferMapping){.dataSize = sizeof(CcoModelViewProjection),
                                                   .dataOffset = offsetof(CcoModelViewProjection, projection),
                                                   .data = &mvpBuffer.projection});

        ccoSetOpenGLViewport((CcoViewport){.x = 0,
                                           .y = 0,
                                           .w = (i32)windowFramebufferSize.w,
                                           .h = (i32)windowFramebufferSize.h,
                                           .minDepth = 0,
                                           .maxDepth = 1});
        ccoSetOpenGLClearColor((CcoClearColor){.r = 0.12f, .g = 0.12f, .b = 0.12f, .a = 1.0f});
        ccoClearOpenGLBuffers(CCO_OPENGL_COLOR_BUFFER_BIT | CCO_OPENGL_DEPTH_BUFFER_BIT);

        ccoUseOpenGLPipeline(pip);
        ccoUseOpenGLVao(vao);
        ccoUseOpenGLUbo(&(CcoOpenGLUboBinding){.type = CCO_OPENGL_UBO_BINDING_BLOCK_NAME, .pip = pip, .name = "MVP"},
                        ubo);
        ccoDrawOpenGLElements(CCO_OPENGL_PRIMITIVE_TRIANGLES, 3, CCO_OPENGL_INDEX_TYPE_U32);

        ccoFlushOpenGLContextBuffer(glCtx);

        ccoSleep(5);
    }

    ccoCloseWindow(window);
    ccoWindowingQuit();
    return 0;
}