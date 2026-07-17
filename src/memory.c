#include "../include/memory.h"
#include "stddef.h"

inline int growCapacity(int oldCap){
    if(oldCap==0)return 8;
    return oldCap*2;
}

void* reallocate(void* pointer , size_t newSize){
    if(newSize == 0){
        free(pointer);
        return NULL;
    }
    void* result = realloc(pointer , newSize);
    if(result==NULL)exit(1);
    return result;
}
void iniVec(Vec *vector , size_t elementSize){
    vector->vector = NULL;
    vector->count =0;
    vector->capacity=0;
    vector->size = elementSize;
}

void* pushVec(Vec *vector){
    if(vector->count == vector->capacity){
        vector->capacity = growCapacity(vector->capacity);
        vector->vector = reallocate(vector->vector ,  vector->capacity * vector->size);
    }
    vector->count++;
    return ((char*)vector->vector)+ ((vector->count -1)*vector->size);
}

void popVec(Vec *vector){
    if(!vector->count)return;
    vector->count--;
}
