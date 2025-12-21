#pragma once

#include "core/core_types.h"

typedef void *CcoDynamicArrayObj;

typedef CcoDynamicArrayObj (*CcoDynamicArrayObjConstructorFun)();
typedef void (*CcoDynamicArrayObjDestructorFun)(CcoDynamicArrayObj *object);

typedef struct cco_dynamic_array cco_dynamic_array;

cco_dynamic_array *cco_create_dynamic_array(u32 reserve, usize size, CcoDynamicArrayObjConstructorFun constructor,
                                     CcoDynamicArrayObjDestructorFun destructor);
cco_dynamic_array *cco_create_dynamic_array_from_array(u32 *count, usize size, CcoDynamicArrayObj *array);
void cco_destroy_dynamic_array(cco_dynamic_array *dyn_array);

void cco_resize_dynamic_array(cco_dynamic_array *dyn_array, usize count);

CcoDynamicArrayObj cco_emplace_back_dynamic_array(cco_dynamic_array *dyn_array);

void cco_add_to_dynamic_array(cco_dynamic_array *dyn_array, usize pos, CcoDynamicArrayObj object);
void cco_remove_from_dynamic_array(cco_dynamic_array *dyn_array, usize pos);

void cco_push_back_array_dynamic_array(cco_dynamic_array *dyn_array, CcoDynamicArrayObj *array, u32 array_count);
void cco_push_back_dynamic_array(cco_dynamic_array *dyn_array, CcoDynamicArrayObj obj);
void cco_pop_back_dynamic_array(cco_dynamic_array *dyn_array);

void cco_clear_dynamic_array(cco_dynamic_array *dyn_array);

CcoBool cco_has_object_in_dynamic_array(cco_dynamic_array *dyn_array, CcoDynamicArrayObj obj);
CcoBool cco_is_dynamic_array_empty(cco_dynamic_array *dyn_array);

CcoDynamicArrayObj *cco_get_dynamic_array_objects(cco_dynamic_array *dyn_array);
CcoDynamicArrayObj *cco_get_dynamic_array_object(cco_dynamic_array *dyn_array, usize pos);
CcoDynamicArrayObj cco_get_dynamic_array_object_back(cco_dynamic_array *dyn_array);
CcoDynamicArrayObj cco_get_dynamic_array_object_front(cco_dynamic_array *dyn_array);

u32 cco_get_dynamic_array_count(cco_dynamic_array *dyn_array);
u32 cco_get_dynamic_array_capacity(cco_dynamic_array *dyn_array);