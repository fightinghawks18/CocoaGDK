#include "tools/resource_manager.h"
#include <stdlib.h>

#include "tools/dynamic_array.h"

typedef struct {
    u64 id;
    void *resource;
    bool active;
} CcoResourceSlot;

struct CcoResourceHandle {
    u64 id;
};

struct CcoResourceManager {
    CcoDynamicArray *slotArray;
    CcoDynamicArray *freeArray;
};

u64 createId(u32 index, u32 gen) { return (u64)index << 32 | gen; }

u32 extractIndex(u64 id) { return (u32)(id >> 32); }

u32 extractGeneration(u64 id) { return (u32)id; }

void invalidateHandle(CcoResourceHandle *handle) { handle->id = UINT64_MAX; }

bool validateHandle(CcoResourceManager *resourceManager, CcoResourceHandle handle) {
    u32 index_handle = extractIndex(handle.id);
    CcoResourceSlot *slot = (CcoResourceSlot *)ccoGetDynamicArrayObject(resourceManager->slotArray, index_handle);
    u32 index_slot = extractIndex(slot->id);
    u32 gen_handle = extractGeneration(handle.id);
    u32 gen_slot = extractGeneration(slot->id);
    return index_handle == index_slot && gen_handle == gen_slot;
}

CcoDynamicArrayObj createResourceSlot(void) {
    CcoResourceSlot *slot = malloc(sizeof(CcoResourceSlot));
    if (slot == NULL) {
        return NULL;
    }
    
    slot->id = UINT64_MAX;
    slot->resource = NULL;
    slot->active = false;
    
    return slot;
}

CcoResourceManager *ccoCreateResourceManager() {
    CcoResourceManager *resource_manager = malloc(sizeof(CcoResourceManager));
    
    resource_manager->slotArray = ccoCreateDynamicArray(100, sizeof(CcoResourceHandle), createResourceSlot, NULL);
    resource_manager->freeArray = ccoCreateDynamicArray(100, sizeof(u32), NULL, NULL);

    return resource_manager;
}

void ccoDestroyResourceManager(CcoResourceManager *resourceManager) {
    ccoDestroyDynamicArray(resourceManager->slotArray);
    ccoDestroyDynamicArray(resourceManager->freeArray);
    free(resourceManager);
}

CcoResourceHandle ccoAllocateResource(CcoResourceManager *resourceManager, usize size) {
    u32 index = 0;
    if (ccoIsDynamicArrayEmpty(resourceManager->freeArray)) {
        ccoEmplaceBackDynamicArray(resourceManager->slotArray);
        u32 count = ccoGetDynamicArrayCount(resourceManager->slotArray);
        index = count - 1;
    } else {
        u32 count = ccoGetDynamicArrayCount(resourceManager->freeArray);
        index = (u32)(uintptr_t)ccoGetDynamicArrayObject(resourceManager->freeArray, count - 1);
        ccoPopBackDynamicArray(resourceManager->freeArray);
    }

    CcoResourceSlot *slot = (CcoResourceSlot*)ccoGetDynamicArrayObject(resourceManager->slotArray, index);
    slot->resource = malloc(size);

    u32 gen = 0;
    if (slot->id != UINT64_MAX) {
        gen = extractGeneration(slot->id);
    }
    slot->id = createId(index, gen);
    slot->active = true;
    return (CcoResourceHandle){slot->id};
}

void ccoDestroyResource(CcoResourceManager *resourceManager, CcoResourceHandle *handle) {
    if (!validateHandle(resourceManager, *handle)) {
        invalidateHandle(handle);
        return;
    }

    CcoResourceSlot *slot = (CcoResourceSlot*)ccoGetDynamicArrayObject(resourceManager->slotArray, extractIndex(handle->id));
    free(slot->resource);
    slot->resource = NULL;
    slot->active = false;

    u32 gen = extractGeneration(slot->id) + 1;
    u32 index = extractIndex(slot->id);
    slot->id = createId(index, gen);
    invalidateHandle(handle);
    ccoPushBackDynamicArray(resourceManager->freeArray, (void*)(uintptr_t)index);
}

void *ccoResolveResource(CcoResourceManager *resourceManager, CcoResourceHandle *handle) {
    if (!validateHandle(resourceManager, *handle)) {
        invalidateHandle(handle);
        return NULL;
    }

    CcoResourceSlot *slot = (CcoResourceSlot*)ccoGetDynamicArrayObject(resourceManager->slotArray, extractIndex(handle->id));
    return slot->resource;
}
