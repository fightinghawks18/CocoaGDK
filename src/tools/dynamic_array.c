#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>

struct EvDynamicArray {
    EvDynamicArrayObj *objs;
    u32 count;
    u32 capacity;
    usize arrayTypeSize;
    EvDynamicArrayObjConstructorFun ctor;
    EvDynamicArrayObjDestructorFun dtor;
};

void addMember(EvDynamicArray *dynArray, usize pos, EvDynamicArrayObj object) {
    if (object == NULL)
        return;

    if (dynArray->count >= dynArray->capacity) {
        usize newCapacity = dynArray->capacity == 0 ? 1 : dynArray->capacity * 2;
        EvDynamicArrayObj *newObjs = realloc(dynArray->objs, newCapacity * sizeof(EvDynamicArrayObj));
        if (newObjs == NULL)
            return;
        dynArray->objs = newObjs;

        printf("Resized from %d to %zu\n", dynArray->capacity, newCapacity);
        dynArray->capacity = newCapacity;
    }

    if (pos > dynArray->count)
        pos = dynArray->count;

    // Move members to the right if they are in or after this position
    if (pos < dynArray->count) {
        for (usize i = dynArray->count; i > pos; i--) {
            dynArray->objs[i] = dynArray->objs[i - 1];
        }
    }

    dynArray->objs[pos] = object;
    dynArray->count++;

    printf("Added member %d\n", dynArray->count);
}

void removeMember(EvDynamicArray *dynArray, usize pos) {
    EvDynamicArrayObj obj = dynArray->objs[pos];
    if (obj == NULL)
        return;

    if (dynArray->dtor != NULL) {
        dynArray->dtor(obj);
    } else {
        free(obj);
    }

    // Move members to the left if they come after this position
    if (pos < dynArray->count) {
        for (usize i = pos + 1; i < dynArray->count; i++) {
            dynArray->objs[i - 1] = dynArray->objs[i];
        }
    }

    dynArray->objs[dynArray->count - 1] = NULL;
    dynArray->count--;
}

EvDynamicArray *evCreateDynamicArray(u32 reserve, usize size, EvDynamicArrayObjConstructorFun constructor,
                                     EvDynamicArrayObjDestructorFun destructor) {
    EvDynamicArray *dynArray = malloc(1 * sizeof(EvDynamicArray));

    dynArray->capacity = reserve;
    dynArray->count = 0;
    dynArray->objs = malloc(reserve * size);
    dynArray->arrayTypeSize = size;
    dynArray->ctor = constructor;
    dynArray->dtor = destructor;

    return dynArray;
}

void evDestroyDynamicArray(EvDynamicArray *dynArray) {
    evClearDynamicArray(dynArray);
    free(dynArray->objs);
    free(dynArray);
}

EvDynamicArrayObj evEmplaceBackDynamicArray(EvDynamicArray *dynArray) {
    EvDynamicArrayObj obj;
    if (dynArray->ctor != NULL) {
        obj = dynArray->ctor();
    } else {
        obj = malloc(dynArray->arrayTypeSize);
    }
    addMember(dynArray, evGetDynamicArrayCount(dynArray), obj);
    return obj;
}

void evAddToDynamicArray(EvDynamicArray *dynArray, usize pos, EvDynamicArrayObj object) {
    addMember(dynArray, pos, object);
}
void evRemoveFromDynamicArray(EvDynamicArray *dynArray, usize pos) { removeMember(dynArray, pos); }

void evPushBackDynamicArray(EvDynamicArray *dynArray, EvDynamicArrayObj object) {
    addMember(dynArray, evGetDynamicArrayCount(dynArray), object);
}
void evPopBackDynamicArray(EvDynamicArray *dynArray) { removeMember(dynArray, evGetDynamicArrayCount(dynArray) - 1); }

void evClearDynamicArray(EvDynamicArray *dynArray) {
    for (u32 i = 0; i < dynArray->count; i++) {
        removeMember(dynArray, i);
    }
    dynArray->count = 0;
}

bool evHasObjectInDynamicArray(EvDynamicArray *dynArray, EvDynamicArrayObj obj) {
    for (u32 i = 0; i < dynArray->count; i++) {
        if (obj == dynArray->objs[i])
            return true;
    }
    return false;
}

EvDynamicArrayObj *evGetDynamicArrayObjects(EvDynamicArray *dynArray) { return dynArray->objs; }

bool evIsDynamicArrayEmpty(EvDynamicArray *dynArray) {
    return dynArray->count == 0;
}

EvDynamicArrayObj *evGetDynamicArrayObject(EvDynamicArray *dynArray, usize pos) {
    if (pos >= dynArray->count)
        return NULL;
    return &dynArray->objs[pos];
}

EvDynamicArrayObj evGetDynamicArrayObjectBack(EvDynamicArray *dynArray) { return dynArray->objs[dynArray->count - 1]; }

EvDynamicArrayObj evGetDynamicArrayObjectFront(EvDynamicArray *dynArray) { return dynArray->objs[0]; }

u32 evGetDynamicArrayCount(EvDynamicArray *dynArray) { return dynArray->count; }

u32 evGetDynamicArrayCapacity(EvDynamicArray *dynArray) { return dynArray->capacity; }