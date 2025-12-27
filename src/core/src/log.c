//
// Created by fightinghawks18 on 12/26/2025.
//

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "core/log.h"


#ifdef _WIN32
#define LOG_DIR "C:\\temp\\"
#else
#define LOG_DIR "/tmp/"
#endif

static char g_log_file_name[256] = "cocoa_debug.log";

void cco_set_log_file_name(const char *name) {
    strncpy(g_log_file_name, name, sizeof(g_log_file_name) - 1);
    g_log_file_name[sizeof(g_log_file_name) - 1] = '\0';
}

void cco_log_to_file(const char *fmt, ...) {
    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s%s", LOG_DIR, g_log_file_name);
    FILE *f = fopen(full_path, "a");
    if (!f)
        return;

    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);
    fprintf(f, "\n");
    fclose(f);
}
