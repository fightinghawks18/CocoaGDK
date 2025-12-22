//
// Created by fightinghawks18 on 12/15/2025.
//

#include "opengl/opengl_loader.h"

#include <windows.h>

void *cco_get_opengl_proc_addr(const char *addr_name) {
    // Test WGL for extensions first
    void *proc = (void*)wglGetProcAddress(addr_name);
    if (proc) {
        return proc;
    }

    // Fall back to GetProcAddress if WGL failed
    static HMODULE opengl32 = NULL;
    if (!opengl32) {
        opengl32 = LoadLibraryA("opengl32.dll");
    }

    return (void*)GetProcAddress(opengl32, addr_name);
}