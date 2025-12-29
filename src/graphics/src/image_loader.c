//
// Created by fightinghawks18 on 12/29/2025.
//

#include "graphics/image_loader.h"
#include "core/log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

cco_result cco_load_image_from_file(const char *path, cco_image **out_image) {
    i32 width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 4);
    if (!data) {
        CCO_LOG("Failed to load %s from path as an image!", path);
        return CCO_FAIL_FILE_NON_EXISTENT;
    }

    cco_image *image = malloc(sizeof(cco_image));
    if (!image) {
        free(data);
        return CCO_FAIL_OUT_OF_MEMORY;
    }

    image->width = width;
    image->height = height;
    image->channel = channels;
    image->pixels = data;

    *out_image = image;
    return CCO_SUCCESS;
}

void cco_free_image(cco_image *image) {
    if (image->pixels) {
        free(image->pixels);
        image->pixels = CCO_NIL;
    }
    free(image);
}
