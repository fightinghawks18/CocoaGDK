#include <stdio.h>

#include "opengl/include/opengl/gl/opengl_context.h"
#include "opengl/include/opengl/gl/opengl_ebo.h"
#include "opengl/include/opengl/gl/opengl_shader.h"
#include "opengl/include/opengl/gl/opengl_ubo.h"
#include "opengl/gl/opengl_core.h"
#include "opengl/gl/opengl_pipeline.h"
#include "opengl/gl/opengl_vao.h"
#include "platform/utils.h"
#include "platform/window.h"

int main() {
    if (ccoWindowingInit() != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize windowing!");
        return -1;
    }

    CcoWindow window = CCO_NIL;
    CcoResult windowResult = ccoCreateWindow(0, 0, 800, 600, "cocoa", &window);
    if (windowResult != CCO_SUCCESS) {
        CCO_LOG("Failed to create window!");
        return -1;
    }

    CcoOpenGLContext glCtx = CCO_NIL;
    ccoCreateOpenGLContext(ccoWindowGetHandle(window), NULL, &glCtx);
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

    Mat4 modelMatrix = ccoMat4Model(
        ccoMat4Translation(position), ccoMat4Rotation(rotation), ccoMat4Scale(scale));
    Mat4 viewMatrix = ccoMat4Eye(cameraPosition, ccoVec3(0, 0, 0), ccoVec3Up());
    Mat4 projectionMatrix =
        ccoMat4Perspective(CCO_NO, CCO_NO, ccoDegToRad(80.0f), 800.0f / 600.0f, 0.001f, 100.0f);

    CcoModelViewProjection mvpBuffer = {.model = ccoMat4Transpose(modelMatrix),
                                        .view = ccoMat4Transpose(viewMatrix),
                                        .projection = ccoMat4Transpose(projectionMatrix)};

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

    while (!ccoWindowWillClose(window)) {
        ccoWindowPumpEvents(window);

        ccoMakeCurrentOpenGLContext(glCtx);

        CcoWindowContentSize windowContentSize = ccoWindowGetContentSize(window);

        projectionMatrix =
            ccoMat4Perspective(CCO_NO, CCO_NO, ccoDegToRad(80.0f),
                                          (f32)windowContentSize.width / (f32)windowContentSize.height, 0.001f, 100.0f);
        mvpBuffer.projection = ccoMat4Transpose(projectionMatrix);

        ccoMapToOpenGLUbo(ubo, &(CcoBufferMapping){.dataSize = sizeof(CcoModelViewProjection),
                                                   .dataOffset = offsetof(CcoModelViewProjection, projection),
                                                   .data = &mvpBuffer.projection});

        ccoSetOpenGLViewport((CcoViewport){.x = 0,
                                           .y = 0,
                                           .w = (i32)windowContentSize.width,
                                           .h = (i32)windowContentSize.height,
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

    ccoDestroyWindow(window);
    ccoWindowingQuit();
    return 0;
}
