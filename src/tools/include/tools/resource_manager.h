#pragma once

#include "core/core_types.h"

typedef struct cco_resource_handle cco_resource_handle;
typedef struct cco_resource_manager cco_resource_manager;

cco_resource_manager* cco_create_resource_manager();
void cco_destroy_resource_manager(cco_resource_manager* resource_manager);

cco_resource_handle cco_allocate_resource(cco_resource_manager* resource_manager, usize size);
void cco_destroy_resource(cco_resource_manager* resource_manager, cco_resource_handle* handle);

void* cco_resolve_resource(cco_resource_manager* resource_manager, cco_resource_handle* handle);
