//
// Created by fightinghawks18 on 12/14/25.
//

#include "opengl/opengl_loader.h"
#include <dlfcn.h>
#include "core/core_types.h"

void *ccoGetGLProcAddr(const char *addrName) {
    static void* opengl_lib = NULL;
    if (!opengl_lib) {
        opengl_lib = dlopen("/System/Library/Frameworks/OpenGL.framework/OpenGL", RTLD_LAZY);
    }
    return opengl_lib ? dlsym(opengl_lib, addrName) : NULL;
}