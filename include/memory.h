#pragma once
#include <stddef.h>
#include "discky.h"

# define pushVector(vec , data) \ (*(typeof(data)*)pushVec(vec) = data) 

typedef struct{
    void* vector;
    int capacity;
    int count;
    int size;
}Vec;

void iniVec(Vec *vector , size_t elementSize);
void* pushVec(Discky* discky , Vec *vector);
void* getVecElement(Discky* discky , Vec* vector , int x);
void resizeVec(Discky* discky , Vec* vector , int x);