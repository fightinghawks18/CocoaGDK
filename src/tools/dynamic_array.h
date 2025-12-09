#pragma once

#include "../core.h"

typedef void *CcoDynamicArrayObj;

typedef CcoDynamicArrayObj (*CcoDynamicArrayObjConstructorFun)();
typedef void (*CcoDynamicArrayObjDestructorFun)(CcoDynamicArrayObj *object);

typedef struct CcoDynamicArray CcoDynamicArray;

CcoDynamicArray *ccoCreateDynamicArray(u32 reserve, usize size, CcoDynamicArrayObjConstructorFun constructor,
                                     CcoDynamicArrayObjDestructorFun destructor);
void ccoDestroyDynamicArray(CcoDynamicArray *dynArray);

void ccoResizeDynamicArray(CcoDynamicArray *dynArray, usize count);

CcoDynamicArrayObj ccoEmplaceBackDynamicArray(CcoDynamicArray *dynArray);

void ccoAddToDynamicArray(CcoDynamicArray *dynArray, usize pos, CcoDynamicArrayObj object);
void ccoRemoveFromDynamicArray(CcoDynamicArray *dynArray, usize pos);

void ccoPushBackDynamicArray(CcoDynamicArray *dynArray, CcoDynamicArrayObj object);
void ccoPopBackDynamicArray(CcoDynamicArray *dynArray);

void ccoClearDynamicArray(CcoDynamicArray *dynArray);

bool ccoHasObjectInDynamicArray(CcoDynamicArray *dynArray, CcoDynamicArrayObj obj);
bool ccoIsDynamicArrayEmpty(CcoDynamicArray *dynArray);

CcoDynamicArrayObj *ccoGetDynamicArrayObjects(CcoDynamicArray *dynArray);
CcoDynamicArrayObj *ccoGetDynamicArrayObject(CcoDynamicArray *dynArray, usize pos);
CcoDynamicArrayObj ccoGetDynamicArrayObjectBack(CcoDynamicArray *dynArray);
CcoDynamicArrayObj ccoGetDynamicArrayObjectFront(CcoDynamicArray *dynArray);

u32 ccoGetDynamicArrayCount(CcoDynamicArray *dynArray);
u32 ccoGetDynamicArrayCapacity(CcoDynamicArray *dynArray);