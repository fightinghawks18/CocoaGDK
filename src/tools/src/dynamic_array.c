#include "tools/dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>

struct cco_dynamic_array {
    CcoDynamicArrayObj *objs;
    u32 count;
    u32 capacity;
    usize array_type_size;
    CcoDynamicArrayObjConstructorFun ctor;
    CcoDynamicArrayObjDestructorFun dtor;
};

void add_member(cco_dynamic_array *dyn_array, usize pos, CcoDynamicArrayObj object) {
    if (object == NULL)
        return;

    if (dyn_array->count >= dyn_array->capacity) {
        usize new_capacity = dyn_array->capacity == 0 ? 1 : dyn_array->capacity * 2;
        CcoDynamicArrayObj *new_objs = realloc(dyn_array->objs, new_capacity * sizeof(CcoDynamicArrayObj));
        if (new_objs == NULL)
            return;
        dyn_array->objs = new_objs;

        printf("Resized from %d to %zu\n", dyn_array->capacity, new_capacity);
        dyn_array->capacity = new_capacity;
    }

    if (pos > dyn_array->count)
        pos = dyn_array->count;

    // Move members to the right if they are in or after this position
    if (pos < dyn_array->count) {
        for (usize i = dyn_array->count; i > pos; i--) {
            dyn_array->objs[i] = dyn_array->objs[i - 1];
        }
    }

    dyn_array->objs[pos] = object;
    dyn_array->count++;
}

void remove_member(cco_dynamic_array *dyn_array, usize pos) {
    CcoDynamicArrayObj obj = dyn_array->objs[pos];
    if (obj == NULL)
        return;

    if (dyn_array->dtor != NULL) {
        dyn_array->dtor(obj);
    }

    // Move members to the left if they come after this position
    if (pos < dyn_array->count) {
        for (usize i = pos + 1; i < dyn_array->count; i++) {
            dyn_array->objs[i - 1] = dyn_array->objs[i];
        }
    }

    dyn_array->objs[dyn_array->count - 1] = NULL;
    dyn_array->count--;
}

cco_dynamic_array *cco_create_dynamic_array(u32 reserve, usize size, CcoDynamicArrayObjConstructorFun constructor,
                                     CcoDynamicArrayObjDestructorFun destructor) {
    cco_dynamic_array *dyn_array = malloc(1 * sizeof(cco_dynamic_array));

    dyn_array->capacity = reserve;
    dyn_array->count = 0;
    dyn_array->objs = malloc(reserve * size);
    dyn_array->array_type_size = size;
    dyn_array->ctor = constructor;
    dyn_array->dtor = destructor;

    return dyn_array;
}

cco_dynamic_array *cco_create_dynamic_array_from_array(u32 *count, usize size, CcoDynamicArrayObj *array) {
    cco_dynamic_array *dyn_array = malloc(1 * sizeof(cco_dynamic_array));

    dyn_array->capacity = *count;
    dyn_array->count = *count;
    dyn_array->objs = array;
    dyn_array->array_type_size = size;
    dyn_array->ctor = NULL;
    dyn_array->dtor = NULL;

    return dyn_array;
}

void cco_destroy_dynamic_array(cco_dynamic_array *dyn_array) {
    cco_clear_dynamic_array(dyn_array);
    free(dyn_array->objs);
    free(dyn_array);
}

CcoDynamicArrayObj cco_emplace_back_dynamic_array(cco_dynamic_array *dyn_array) {
    CcoDynamicArrayObj obj;
    if (dyn_array->ctor != NULL) {
        obj = dyn_array->ctor();
    } else {
        obj = malloc(dyn_array->array_type_size);
    }
    add_member(dyn_array, cco_get_dynamic_array_count(dyn_array), obj);
    return obj;
}

void cco_add_to_dynamic_array(cco_dynamic_array *dyn_array, usize pos, CcoDynamicArrayObj object) {
    add_member(dyn_array, pos, object);
}
void cco_remove_from_dynamic_array(cco_dynamic_array *dyn_array, usize pos) { remove_member(dyn_array, pos); }

void cco_push_back_array_dynamic_array(cco_dynamic_array *dyn_array, CcoDynamicArrayObj *array, u32 array_count) {
    for (u32 i = 0; i < array_count; i++) {
        cco_push_back_dynamic_array(dyn_array, &array[i]);
    }
}

void cco_push_back_dynamic_array(cco_dynamic_array *dyn_array, CcoDynamicArrayObj object) {
    add_member(dyn_array, cco_get_dynamic_array_count(dyn_array), object);
}
void cco_pop_back_dynamic_array(cco_dynamic_array *dyn_array) { remove_member(dyn_array, cco_get_dynamic_array_count(dyn_array) - 1); }

void cco_clear_dynamic_array(cco_dynamic_array *dyn_array) {
    for (u32 i = 0; i < dyn_array->count; i++) {
        remove_member(dyn_array, i);
    }
    dyn_array->count = 0;
}

CcoBool cco_has_object_in_dynamic_array(cco_dynamic_array *dyn_array, CcoDynamicArrayObj obj) {
    for (u32 i = 0; i < dyn_array->count; i++) {
        if (obj == dyn_array->objs[i])
            return CCO_YES;
    }
    return CCO_NO;
}

CcoDynamicArrayObj *cco_get_dynamic_array_objects(cco_dynamic_array *dyn_array) { return dyn_array->objs; }

CcoBool cco_is_dynamic_array_empty(cco_dynamic_array *dyn_array) {
    return dyn_array->count == 0;
}

CcoDynamicArrayObj *cco_get_dynamic_array_object(cco_dynamic_array *dyn_array, usize pos) {
    if (pos >= dyn_array->count)
        return NULL;
    return dyn_array->objs[pos];
}

CcoDynamicArrayObj cco_get_dynamic_array_object_back(cco_dynamic_array *dyn_array) { return dyn_array->objs[dyn_array->count - 1]; }

CcoDynamicArrayObj cco_get_dynamic_array_object_front(cco_dynamic_array *dyn_array) { return dyn_array->objs[0]; }

u32 cco_get_dynamic_array_count(cco_dynamic_array *dyn_array) { return dyn_array->count; }

u32 cco_get_dynamic_array_capacity(cco_dynamic_array *dyn_array) { return dyn_array->capacity; }