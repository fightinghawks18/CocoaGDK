#pragma once

#include "../core.h"

typedef struct EvResourceHandle EvResourceHandle;
typedef struct EvResourceManager EvResourceManager;

EvResourceManager* evCreateResourceManager();
void evDestroyResourceManager(EvResourceManager* resourceManager);

EvResourceHandle evAllocateResource(EvResourceManager* resourceManager, usize size);
void evDestroyResource(EvResourceManager* resourceManager, EvResourceHandle* handle);

void* evResolveResource(EvResourceManager* resourceManager, EvResourceHandle* handle);
