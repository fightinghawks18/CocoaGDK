#pragma once

#include "../core.h"

typedef struct CcoResourceHandle CcoResourceHandle;
typedef struct CcoResourceManager CcoResourceManager;

CcoResourceManager* ccoCreateResourceManager();
void ccoDestroyResourceManager(CcoResourceManager* resourceManager);

CcoResourceHandle ccoAllocateResource(CcoResourceManager* resourceManager, usize size);
void ccoDestroyResource(CcoResourceManager* resourceManager, CcoResourceHandle* handle);

void* ccoResolveResource(CcoResourceManager* resourceManager, CcoResourceHandle* handle);
