//
// Created by fightinghawks18 on 12/24/2025.
//

#include "cocoa_gdk/vulkan/internal/buffer_vulkan.h"

#include <stdlib.h>
#include <string.h>

cco_result cco_create_vulkan_buffer(cco_vulkan_instance instance, const cco_vulkan_buffer_desc *desc,
                                    cco_vulkan_buffer *out_buffer) {
    cco_vulkan_buffer buffer = malloc(sizeof(cco_vulkan_buffer_t));
    if (!buffer)
        return CCO_FAIL_OUT_OF_MEMORY;

    buffer->instance = instance;
    buffer->buffer = CCO_NIL;
    buffer->allocation = CCO_NIL;

    VkBufferCreateInfo buffer_create_info = {};
    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size = desc->size;
    buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buffer_create_info.pQueueFamilyIndices = NULL;
    buffer_create_info.queueFamilyIndexCount = 0;

    switch (desc->usage) {
    case CCO_VULKAN_BUFFER_USAGE_VERTEX_BIT: {
        buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        break;
    }
    case CCO_VULKAN_BUFFER_USAGE_INDEX_BIT: {
        buffer_create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        break;
    }
    case CCO_VULKAN_BUFFER_USAGE_UNIFORM_BIT: {
        buffer_create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        break;
    }
    case CCO_VULKAN_BUFFER_USAGE_STORAGE_BIT: {
        buffer_create_info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        break;
    }
    default: break;
    }

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

    const VkResult buffer_create_result = vmaCreateBuffer(instance->allocator, &buffer_create_info, &alloc_info,
                                                          &buffer->buffer, &buffer->allocation, &buffer->allocationInfo);
    if (buffer_create_result != VK_SUCCESS) {
        CCO_LOG("Vulkan failed to create handle for buffer (%p)!", buffer);
        cco_destroy_vulkan_buffer(buffer);
        return CCO_FAIL_GRAPHICS_CREATE_ERROR;
    }

    *out_buffer = buffer;
    return CCO_SUCCESS;
}

void cco_destroy_vulkan_buffer(cco_vulkan_buffer buffer) {
    if (buffer->buffer) {
        vmaDestroyBuffer(buffer->instance->allocator, buffer->buffer, buffer->allocation);
        buffer->buffer = CCO_NIL;
        buffer->allocation = CCO_NIL;
    }
    free(buffer);
}

void cco_vulkan_buffer_map_data(cco_vulkan_buffer buffer, const cco_buffer_mapping *data) {
    memcpy(buffer->allocationInfo.pMappedData, &data->data, data->data_size);
}
