//
// Created by fightinghawks18 on 12/29/2025.
//

#include "opengl/opengl_image.h"

#include "opengl/opengl_loader.h"
#include "opengl/opengl_types.h"
#include <stdlib.h>

struct cco_opengl_image_t {
    u32 gl_id;
    i32 target;
};

CCO_OPENGL_API cco_result cco_create_opengl_image(cco_opengl_image *out_image) {
    cco_opengl_image image = malloc(sizeof(cco_opengl_image_t));
    if (!image)
        return CCO_FAIL_OUT_OF_MEMORY;

    glGenTextures(1, &image->gl_id);

    *out_image = image;
    return CCO_SUCCESS;
}

CCO_OPENGL_API void cco_destroy_opengl_image(cco_opengl_image image) {
    if (image->gl_id != CCO_NULL_GLID) {
        glDeleteTextures(1, &image->gl_id);
        image->gl_id = CCO_NULL_GLID;
    }
    free(image);
}

CCO_OPENGL_API void cco_opengl_image_use(const i32 texture_type, cco_opengl_image image) {
    if (image) {
        glBindTexture(texture_type, image->gl_id);
        image->target = texture_type;
        return;
    }
    glBindTexture(texture_type, CCO_NULL_GLID);
}

void cco_opengl_image_bind(const u32 texture_slot, cco_opengl_image image) {
    glActiveTexture(GL_TEXTURE0 + texture_slot);
    cco_opengl_image_use(image->target, image);
}

CCO_OPENGL_API void cco_opengl_image_upload(const i32 texture_type, cco_opengl_image image, const cco_image *image_data) {
    cco_opengl_image_use(texture_type, image);

    i32 internal_format = 0;
    i32 format = 0;

    switch (image_data->channel) {
    case CCO_IMAGE_CHANNEL_GRAYSCALE: {
        internal_format = GL_R8;
        format = GL_RED;
        break;
    }
    case CCO_IMAGE_CHANNEL_GRAYSCALE_ALPHA: {
        internal_format = GL_RG8;
        format = GL_RG;
        break;
    }
    case CCO_IMAGE_CHANNEL_RGB: {
        internal_format = GL_RGB8;
        format = GL_RGB;
        break;
    }
    case CCO_IMAGE_CHANNEL_RGB_ALPHA: {
        internal_format = GL_RGBA8;
        format = GL_RGBA;
        break;
    }
    default: break;
    }

    if ((image_data->width * image_data->channel) % 4 != 0) // If the image's width * channel isn't divisible by 4 it may cause artifacts
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(image->target, 0, internal_format, image_data->width, image_data->height, 0, format, GL_UNSIGNED_BYTE, image_data->pixels);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

CCO_OPENGL_API void cco_opengl_image_set_wrapping(cco_opengl_image image, const i32 wrap_s, const i32 wrap_t) {
    glTexParameteri(image->target, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(image->target, GL_TEXTURE_WRAP_T, wrap_t);
}

CCO_OPENGL_API void cco_opengl_image_set_filtering(cco_opengl_image image, const i32 min_filter, const i32 mag_filter) {
    glTexParameteri(image->target, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(image->target, GL_TEXTURE_MAG_FILTER, mag_filter);

    if (min_filter == GL_LINEAR_MIPMAP_LINEAR || min_filter == GL_LINEAR_MIPMAP_NEAREST
        || mag_filter == GL_LINEAR_MIPMAP_LINEAR || mag_filter == GL_LINEAR_MIPMAP_NEAREST) {
        glGenerateMipmap(image->target);
    }
}