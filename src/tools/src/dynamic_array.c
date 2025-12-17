#include "tools/dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>

struct CcoDynamicArray {
    CcoDynamicArrayObj *objs;
    u32 count;
    u32 capacity;
    usize arrayTypeSize;
    CcoDynamicArrayObjConstructorFun ctor;
    CcoDynamicArrayObjDestructorFun dtor;
};

void addMember(CcoDynamicArray *dynArray, usize pos, CcoDynamicArrayObj object) {
    if (object == NULL)
        return;

    if (dynArray->count >= dynArray->capacity) {
        usize newCapacity = dynArray->capacity == 0 ? 1 : dynArray->capacity * 2;
        CcoDynamicArrayObj *newObjs = realloc(dynArray->objs, newCapacity * sizeof(CcoDynamicArrayObj));
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
}

void removeMember(CcoDynamicArray *dynArray, usize pos) {
    CcoDynamicArrayObj obj = dynArray->objs[pos];
    if (obj == NULL)
        return;

    if (dynArray->dtor != NULL) {
        dynArray->dtor(obj);
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

CcoDynamicArray *ccoCreateDynamicArray(u32 reserve, usize size, CcoDynamicArrayObjConstructorFun constructor,
                                     CcoDynamicArrayObjDestructorFun destructor) {
    CcoDynamicArray *dynArray = malloc(1 * sizeof(CcoDynamicArray));

    dynArray->capacity = reserve;
    dynArray->count = 0;
    dynArray->objs = malloc(reserve * size);
    dynArray->arrayTypeSize = size;
    dynArray->ctor = constructor;
    dynArray->dtor = destructor;

    return dynArray;
}

CcoDynamicArray *ccoCreateDynamicArrayFromArray(u32 *count, usize size, CcoDynamicArrayObj *array) {
    CcoDynamicArray *dynArray = malloc(1 * sizeof(CcoDynamicArray));

    dynArray->capacity = *count;
    dynArray->count = *count;
    dynArray->objs = array;
    dynArray->arrayTypeSize = size;
    dynArray->ctor = NULL;
    dynArray->dtor = NULL;

    return dynArray;
}

void ccoDestroyDynamicArray(CcoDynamicArray *dynArray) {
    ccoClearDynamicArray(dynArray);
    free(dynArray->objs);
    free(dynArray);
}

CcoDynamicArrayObj ccoEmplaceBackDynamicArray(CcoDynamicArray *dynArray) {
    CcoDynamicArrayObj obj;
    if (dynArray->ctor != NULL) {
        obj = dynArray->ctor();
    } else {
        obj = malloc(dynArray->arrayTypeSize);
    }
    addMember(dynArray, ccoGetDynamicArrayCount(dynArray), obj);
    return obj;
}

void ccoAddToDynamicArray(CcoDynamicArray *dynArray, usize pos, CcoDynamicArrayObj object) {
    addMember(dynArray, pos, object);
}
void ccoRemoveFromDynamicArray(CcoDynamicArray *dynArray, usize pos) { removeMember(dynArray, pos); }

void ccoPushBackArrayDynamicArray(CcoDynamicArray *dynArray, CcoDynamicArrayObj *array, u32 arrayCount) {
    for (u32 i = 0; i < arrayCount; i++) {
        ccoPushBackDynamicArray(dynArray, &array[i]);
    }
}

void ccoPushBackDynamicArray(CcoDynamicArray *dynArray, CcoDynamicArrayObj object) {
    addMember(dynArray, ccoGetDynamicArrayCount(dynArray), object);
}
void ccoPopBackDynamicArray(CcoDynamicArray *dynArray) { removeMember(dynArray, ccoGetDynamicArrayCount(dynArray) - 1); }

void ccoClearDynamicArray(CcoDynamicArray *dynArray) {
    for (u32 i = 0; i < dynArray->count; i++) {
        removeMember(dynArray, i);
    }
    dynArray->count = 0;
}

CcoBool ccoHasObjectInDynamicArray(CcoDynamicArray *dynArray, CcoDynamicArrayObj obj) {
    for (u32 i = 0; i < dynArray->count; i++) {
        if (obj == dynArray->objs[i])
            return CCO_YES;
    }
    return CCO_NO;
}

CcoDynamicArrayObj *ccoGetDynamicArrayObjects(CcoDynamicArray *dynArray) { return dynArray->objs; }

CcoBool ccoIsDynamicArrayEmpty(CcoDynamicArray *dynArray) {
    return dynArray->count == 0;
}

CcoDynamicArrayObj *ccoGetDynamicArrayObject(CcoDynamicArray *dynArray, usize pos) {
    if (pos >= dynArray->count)
        return NULL;
    return dynArray->objs[pos];
}

CcoDynamicArrayObj ccoGetDynamicArrayObjectBack(CcoDynamicArray *dynArray) { return dynArray->objs[dynArray->count - 1]; }

CcoDynamicArrayObj ccoGetDynamicArrayObjectFront(CcoDynamicArray *dynArray) { return dynArray->objs[0]; }

u32 ccoGetDynamicArrayCount(CcoDynamicArray *dynArray) { return dynArray->count; }

u32 ccoGetDynamicArrayCapacity(CcoDynamicArray *dynArray) { return dynArray->capacity; }