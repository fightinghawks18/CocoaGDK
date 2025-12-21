#include "tools/resource_manager.h"
#include <stdlib.h>

#include "tools/dynamic_array.h"

typedef struct {
    u64 id;
    void *resource;
    CcoBool active;
} cco_resource_slot;

struct cco_resource_handle {
    u64 id;
};

struct cco_resource_manager {
    cco_dynamic_array *slot_array;
    cco_dynamic_array *free_array;
};

u64 create_id(u32 index, u32 gen) { return (u64)index << 32 | gen; }

u32 extract_index(u64 id) { return (u32)(id >> 32); }

u32 extract_generation(u64 id) { return (u32)id; }

void invalidate_handle(cco_resource_handle *handle) { handle->id = UINT64_MAX; }

CcoBool validate_handle(cco_resource_manager *resource_manager, cco_resource_handle handle) {
    u32 index_handle = extract_index(handle.id);
    cco_resource_slot *slot = (cco_resource_slot *)cco_get_dynamic_array_object(resource_manager->slot_array, index_handle);
    u32 index_slot = extract_index(slot->id);
    u32 gen_handle = extract_generation(handle.id);
    u32 gen_slot = extract_generation(slot->id);
    return index_handle == index_slot && gen_handle == gen_slot;
}

CcoDynamicArrayObj create_resource_slot(void) {
    cco_resource_slot *slot = malloc(sizeof(cco_resource_slot));
    if (slot == NULL) {
        return NULL;
    }
    
    slot->id = UINT64_MAX;
    slot->resource = NULL;
    slot->active = CCO_NO;
    
    return slot;
}

cco_resource_manager *cco_create_resource_manager() {
    cco_resource_manager *resource_manager = malloc(sizeof(cco_resource_manager));
    
    resource_manager->slot_array = cco_create_dynamic_array(100, sizeof(cco_resource_handle), create_resource_slot, NULL);
    resource_manager->free_array = cco_create_dynamic_array(100, sizeof(u32), NULL, NULL);

    return resource_manager;
}

void cco_destroy_resource_manager(cco_resource_manager *resource_manager) {
    cco_destroy_dynamic_array(resource_manager->slot_array);
    cco_destroy_dynamic_array(resource_manager->free_array);
    free(resource_manager);
}

cco_resource_handle cco_allocate_resource(cco_resource_manager *resource_manager, usize size) {
    u32 index = 0;
    if (cco_is_dynamic_array_empty(resource_manager->free_array)) {
        cco_emplace_back_dynamic_array(resource_manager->slot_array);
        u32 count = cco_get_dynamic_array_count(resource_manager->slot_array);
        index = count - 1;
    } else {
        u32 count = cco_get_dynamic_array_count(resource_manager->free_array);
        index = (u32)(uintptr_t)cco_get_dynamic_array_object(resource_manager->free_array, count - 1);
        cco_pop_back_dynamic_array(resource_manager->free_array);
    }

    cco_resource_slot *slot = (cco_resource_slot*)cco_get_dynamic_array_object(resource_manager->slot_array, index);
    slot->resource = malloc(size);

    u32 gen = 0;
    if (slot->id != UINT64_MAX) {
        gen = extract_generation(slot->id);
    }
    slot->id = create_id(index, gen);
    slot->active = CCO_YES;
    return (cco_resource_handle){slot->id};
}

void cco_destroy_resource(cco_resource_manager *resource_manager, cco_resource_handle *handle) {
    if (!validate_handle(resource_manager, *handle)) {
        invalidate_handle(handle);
        return;
    }

    cco_resource_slot *slot = (cco_resource_slot*)cco_get_dynamic_array_object(resource_manager->slot_array, extract_index(handle->id));
    free(slot->resource);
    slot->resource = NULL;
    slot->active = CCO_NO;

    u32 gen = extract_generation(slot->id) + 1;
    u32 index = extract_index(slot->id);
    slot->id = create_id(index, gen);
    invalidate_handle(handle);
    cco_push_back_dynamic_array(resource_manager->free_array, (void*)(uintptr_t)index);
}

void *cco_resolve_resource(cco_resource_manager *resource_manager, cco_resource_handle *handle) {
    if (!validate_handle(resource_manager, *handle)) {
        invalidate_handle(handle);
        return NULL;
    }

    cco_resource_slot *slot = (cco_resource_slot*)cco_get_dynamic_array_object(resource_manager->slot_array, extract_index(handle->id));
    return slot->resource;
}
