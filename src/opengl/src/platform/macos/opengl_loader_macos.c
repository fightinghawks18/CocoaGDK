//
// Created by fightinghawks18 on 12/14/25.
//

#include "cocoa_gdk/opengl/opengl_loader.h"
#include "cocoa_gdk/core/core_types.h"
#include "cocoa_gdk/core/results.h"
#include "cocoa_gdk/core/log.h"
#include <dlfcn.h>

void *cco_get_opengl_proc_addr(const char *addr_name) {
    static void* opengl_lib = NULL;
    if (!opengl_lib) {
        opengl_lib = dlopen("/System/Library/Frameworks/OpenGL.framework/OpenGL", RTLD_LAZY);
    }
    return opengl_lib ? dlsym(opengl_lib, addr_name) : NULL;
}