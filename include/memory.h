#pragma once
#include <stddef.h>

# define pushVector(vec , data) \ (*(typeof(data)*)pushVec(vec) = data) 

typedef struct{
    void* vector;
    int capacity;
    int count;
    int size;
}Vec;

void iniVec(Vec *vector , size_t elementSize);
void* pushVec(Vec *vector);