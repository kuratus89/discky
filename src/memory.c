#include "../include/memory.h"

inline int growCapacity(int oldCap){
    if(oldCap==0)return 8;
    return oldCap*2;
}

void* reallocate( Discky* discky, void* pointer , size_t newSize){
    if(newSize == 0){
        free(pointer);
        return NULL;
    }
    void* result = realloc(pointer , newSize);
    if(result==NULL)callErrorHandle(discky , ERROR_OUT_OF_MEM ,"cannot allocate memory");
    return result;
}
void iniVec(Vec *vector , size_t elementSize){
    vector->vector = NULL;
    vector->count =0;
    vector->capacity=0;
    vector->size = elementSize;
}

void* pushVec(Discky* discky ,Vec *vector){
    if(vector->count == vector->capacity){
        vector->capacity = growCapacity(vector->capacity);
        vector->vector = reallocate(discky ,vector->vector ,  vector->capacity * vector->size);
    }
    vector->count++;
    return ((char*)vector->vector)+ ((vector->count -1)*vector->size);
}

void popVec(Vec *vector){
    if(!vector->count)return;
    vector->count--;
}

inline void freeVec(Vec* vector){
    free(vector->vector);
}

void* getVecElement(Discky* discky ,const Vec* vector ,const int x){
    if((vector->count<=x)||(x<0))callErrorHandle(discky , ERROR_INTERNAL , "internal error : invalid vector index");
    return ((char*)vector->vector)+ (x*vector->size);
}

void resizeVec(Discky* discky , Vec* vector ,const int x){
    reallocate(discky , vector , x*vector->size);
    vector->capacity = x;
    if(vector->count>x)vector->count = x;
}

void allocBuffer(Screen** screen){
    *screen = malloc(sizeof(Screen));
}

