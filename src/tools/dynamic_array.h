#pragma once

#include "../core.h"

typedef void *EvDynamicArrayObj;

typedef EvDynamicArrayObj (*EvDynamicArrayObjConstructorFun)();
typedef void (*EvDynamicArrayObjDestructorFun)(EvDynamicArrayObj *object);

typedef struct EvDynamicArray EvDynamicArray;

EvDynamicArray *evCreateDynamicArray(u32 reserve, usize size, EvDynamicArrayObjConstructorFun constructor,
                                     EvDynamicArrayObjDestructorFun destructor);
void evDestroyDynamicArray(EvDynamicArray *dynArray);

void evResizeDynamicArray(EvDynamicArray *dynArray, usize count);

EvDynamicArrayObj evEmplaceBackDynamicArray(EvDynamicArray *dynArray);

void evAddToDynamicArray(EvDynamicArray *dynArray, usize pos, EvDynamicArrayObj object);
void evRemoveFromDynamicArray(EvDynamicArray *dynArray, usize pos);

void evPushBackDynamicArray(EvDynamicArray *dynArray, EvDynamicArrayObj object);
void evPopBackDynamicArray(EvDynamicArray *dynArray);

void evClearDynamicArray(EvDynamicArray *dynArray);

bool evHasObjectInDynamicArray(EvDynamicArray *dynArray, EvDynamicArrayObj obj);
bool evIsDynamicArrayEmpty(EvDynamicArray *dynArray);

EvDynamicArrayObj *evGetDynamicArrayObjects(EvDynamicArray *dynArray);
EvDynamicArrayObj *evGetDynamicArrayObject(EvDynamicArray *dynArray, usize pos);
EvDynamicArrayObj evGetDynamicArrayObjectBack(EvDynamicArray *dynArray);
EvDynamicArrayObj evGetDynamicArrayObjectFront(EvDynamicArray *dynArray);

u32 evGetDynamicArrayCount(EvDynamicArray *dynArray);
u32 evGetDynamicArrayCapacity(EvDynamicArray *dynArray);