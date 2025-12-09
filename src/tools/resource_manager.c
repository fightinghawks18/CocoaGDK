#include "resource_manager.h"
#include <stdlib.h>

#include "dynamic_array.h"

typedef struct {
    u64 id;
    void *resource;
    bool active;
} EvResourceSlot;

struct EvResourceHandle {
    u64 id;
};

struct EvResourceManager {
    EvDynamicArray *slotArray;
    EvDynamicArray *freeArray;
};

u64 createId(u32 index, u32 gen) { return (u64)index << 32 | gen; }

u32 extractIndex(u64 id) { return (u32)(id >> 32); }

u32 extractGeneration(u64 id) { return (u32)id; }

void invalidateHandle(EvResourceHandle *handle) { handle->id = UINT64_MAX; }

bool validateHandle(EvResourceManager *resourceManager, EvResourceHandle handle) {
    u32 index_handle = extractIndex(handle.id);
    EvResourceSlot *slot = (EvResourceSlot *)evGetDynamicArrayObject(resourceManager->slotArray, index_handle);
    u32 index_slot = extractIndex(slot->id);
    u32 gen_handle = extractGeneration(handle.id);
    u32 gen_slot = extractGeneration(slot->id);
    return index_handle == index_slot && gen_handle == gen_slot;
}

EvDynamicArrayObj createResourceSlot(void) {
    EvResourceSlot *slot = malloc(sizeof(EvResourceSlot));
    if (slot == NULL) {
        return NULL;
    }
    
    slot->id = UINT64_MAX;
    slot->resource = NULL;
    slot->active = false;
    
    return (EvDynamicArrayObj)slot;
}

EvResourceManager *evCreateResourceManager() {
    EvResourceManager *resource_manager = malloc(sizeof(EvResourceManager));
    
    resource_manager->slotArray = evCreateDynamicArray(100, sizeof(EvResourceHandle), createResourceSlot, NULL);
    resource_manager->freeArray = evCreateDynamicArray(100, sizeof(u32), NULL, NULL);

    return resource_manager;
}

void evDestroyResourceManager(EvResourceManager *resourceManager) {
    evDestroyDynamicArray(resourceManager->slotArray);
    evDestroyDynamicArray(resourceManager->freeArray);
    free(resourceManager);
}

EvResourceHandle evAllocateResource(EvResourceManager *resourceManager, usize size) {
    u32 index = 0;
    if (evIsDynamicArrayEmpty(resourceManager->freeArray)) {
        evEmplaceBackDynamicArray(resourceManager->slotArray);
        u32 count = evGetDynamicArrayCount(resourceManager->slotArray);
        index = count - 1;
    } else {
        u32 count = evGetDynamicArrayCount(resourceManager->freeArray);
        index = (u32)(uintptr_t)evGetDynamicArrayObject(resourceManager->freeArray, count - 1);
        evPopBackDynamicArray(resourceManager->freeArray);
    }

    EvResourceSlot *slot = (EvResourceSlot*)evGetDynamicArrayObject(resourceManager->slotArray, index);
    slot->resource = malloc(size);

    u32 gen = 0;
    if (slot->id != UINT64_MAX) {
        gen = extractGeneration(slot->id);
    }
    slot->id = createId(index, gen);
    slot->active = true;
    return (EvResourceHandle){slot->id};
}

void evDestroyResource(EvResourceManager *resourceManager, EvResourceHandle *handle) {
    if (!validateHandle(resourceManager, *handle)) {
        invalidateHandle(handle);
        return;
    }

    EvResourceSlot *slot = (EvResourceSlot*)evGetDynamicArrayObject(resourceManager->slotArray, extractIndex(handle->id));
    free(slot->resource);
    slot->resource = NULL;
    slot->active = false;

    u32 gen = extractGeneration(slot->id) + 1;
    u32 index = extractIndex(slot->id);
    slot->id = createId(index, gen);
    invalidateHandle(handle);
    evPushBackDynamicArray(resourceManager->freeArray, (void*)(uintptr_t)index);
}

void *evResolveResource(EvResourceManager *resourceManager, EvResourceHandle *handle) {
    if (!validateHandle(resourceManager, *handle)) {
        invalidateHandle(handle);
        return NULL;
    }

    EvResourceSlot *slot = (EvResourceSlot*)evGetDynamicArrayObject(resourceManager->slotArray, extractIndex(handle->id));
    return slot->resource;
}
