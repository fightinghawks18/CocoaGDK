//
// Created by fightinghawks18 on 12/14/25.
//

#include "../../include/opengl/gl/opengl_shader.h"

#include "opengl/gl/opengl_types.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <errno.h>
#else
#include <sys/errno.h>
#endif

cco_result read_file(const char *filepath, char **source) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        switch (errno) {
        case ENOENT:
            return CCO_FAIL_FILE_NON_EXISTENT;
        case EACCES:
        default:
            return CCO_FAIL_CANT_READ_FILE;
        }
    }

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return CCO_FAIL_OUT_OF_MEMORY;
    }

    const size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';

    fclose(file);
    *source = buffer;
    return CCO_SUCCESS;
}

struct cco_opengl_shader_t {
    u32 gl_id;
};

cco_result cco_create_open_gl_shader(const cco_open_gl_shader_desc *shader_desc, cco_opengl_shader *out_shader) {
    cco_opengl_shader shader = malloc(sizeof(cco_opengl_shader_t));
    if (!shader)
        return CCO_FAIL_OUT_OF_MEMORY;

    u32 shader_type = 0;
    switch (shader_desc->shader_type) {
    case CCO_SHADER_TYPE_VERTEX:
        shader_type = GL_VERTEX_SHADER;
        break;
    case CCO_SHADER_TYPE_PIXEL:
        shader_type = GL_FRAGMENT_SHADER;
        break;
    }

    char *shader_contents = NULL;
    const cco_result read_file_result = read_file(shader_desc->shader_path, &shader_contents);
    if (read_file_result != CCO_SUCCESS) {
        CCO_LOG("Failed to load shader contents for OpenGL!");
        return read_file_result;
    }

    shader->gl_id = glCreateShader(shader_type);
    glShaderSource(shader->gl_id, 1, (const char * const *)&shader_contents, NULL);
    glCompileShader(shader->gl_id);

    free(shader_contents);

    i32 compile_success;
    glGetShaderiv(shader->gl_id, GL_COMPILE_STATUS, &compile_success);
    if (!compile_success) {
        char info_log[512];
        glGetShaderInfoLog(shader->gl_id, 512, NULL, info_log);
        CCO_LOG("Failed to compile GL shader from path %s! %s", shader_desc->shader_path, info_log);
        cco_destroy_open_gl_shader(shader);
        return CCO_FAIL_COMPILE_ERROR;
    }

    *out_shader = shader;
    return CCO_SUCCESS;
}

void cco_destroy_open_gl_shader(cco_opengl_shader shader) {
    if (shader->gl_id != CCO_NULL_GLID) {
        glDeleteShader(shader->gl_id);
        shader->gl_id = CCO_NULL_GLID;
    }
    free(shader);
}

u32 cco_get_open_gl_shader_id(cco_opengl_shader shader) { return shader->gl_id; }
