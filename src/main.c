#include <stdio.h>

#include <glad/glad.h>

#include "opengl/opengl_context.h"
#include "opengl/opengl_loader.h"
#include "platform/windowing.h"

int main() {
    if (ccoWindowingInit() != CCO_SUCCESS) {
        CCO_LOG("Failed to initialize windowing!");
        return -1;
    }

    CcoWindow window;
    CcoResult windowResult =
        ccoCreateWindow(&(CcoWindowDesc){CCO_WINDOW_POS_CENTER, CCO_WINDOW_POS_CENTER, 800, 600, "cocoa",
                                         CCO_WINDOW_FLAG_DECOR_BIT | CCO_WINDOW_FLAG_RESIZE_BIT},
                        &window);
    if (windowResult != CCO_SUCCESS) {
        CCO_LOG("Failed to create window!");
        return -1;
    }


    CcoOpenGLContext glCtx;
    ccoCreateOpenGLContext(ccoGetNativeWindowHandle(window), NULL, &glCtx);
    ccoMakeCurrentOpenGLContext(glCtx);

    if (!gladLoadGLLoader(ccoGetGLProcAddr)) {
        CCO_LOG("Failed to load glad!");
        return -1;
    }

    while (!ccoShouldWindowClose(window)) {
        ccoWindowingPoll();

        glViewport(0, 0, 800, 600);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ccoFlushOpenGLContextBuffer(glCtx);
    }



    ccoCloseWindow(window);
    ccoWindowingQuit();
    return 0;
}